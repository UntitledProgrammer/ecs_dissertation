#ifndef NP_ECS_CORE_H
#define NP_ECS_CORE_H

//Includes:
#include <unordered_map>
#include "../allocators/components.hpp"
#include "../logic/system.h"

namespace np_ecs
{
	struct BaseContainer { };

	template<typename T> struct Container : public BaseContainer
	{
	public:
		//Attributes:
		std::unordered_map<entity_t, size_t> entity_to_index;
		std::vector<T> components;

		//Methods:
		void add_component(entity_t entity, T component)
		{
			if (entity_to_index.find(entity) == entity_to_index.end())
			{
				entity_to_index.emplace(entity, components.size());
				components.push_back(component);
			}

			components[entity_to_index[entity]] = component;
		}

		//Operators:
		T& operator[] (entity_t entity) { return components[entity_to_index[entity]]; }
	};

	class World
	{
	private:
		//Attributes:
		EntityAllocator entity_allocator;
		ComponentAllocator c_author;
		Bitmask<SignatureSize> signature;
		std::unordered_map<hash_t, BaseContainer*> containers;
		std::unordered_map<size_t, ISystem*> systems;

	public:
		//Methods:
		template<typename T, typename... args> Bitmask<SignatureSize> combine_masks()
		{
			Bitmask<SignatureSize> bitmask;

			bitmask.set(c_author.get_id<T>(), true);

			([&]
				{

					bitmask.set(c_author.get_id<args>(), true);
				} (), ...);

			return bitmask;
		}


		void update()
		{
			for (std::pair<size_t, ISystem*> pair : systems)
			{
				pair.second->update();
			}
		}

		template<typename S> ISystem* register_system(Bitmask<SignatureSize> bitmask)
		{
			assert(validate_system(S));

			//Add subset: (If system has not already been added.
			if (systems.find(bitmask.get_decimal()) == systems.end())
			{
				systems.emplace(bitmask.get_decimal(), new S());
				systems[bitmask.get_decimal()]->local_world = this;

				//Add any entities with components to the system.
				for (int i = 0; i < entity_allocator.signatures.size(); i++)
				{
					if (entity_allocator.signatures[i] == bitmask)
					{
						systems[bitmask.get_decimal()]->entities.push_back(i);
					}
					else if (entity_allocator.signatures[i].has_signature(bitmask))
					{
						systems[bitmask.get_decimal()]->entities.push_back(i);
					}
				}
			}

			return systems[bitmask.get_decimal()];
		}

		template<typename T> T& get_component(entity_t entity)
		{
			assert(c_author.is_registered<T>());

			size_t index = c_author.get_id<T>();
			return (*static_cast<Container<T>*>(containers[index]))[entity];
		}

		template<typename T> void add_component(entity_t entity, T component)
		{
			assert(entity_allocator.validate(entity));

			size_t bit_value = c_author.get_id<T>();

			//Register component.
			if (!signature.has_bit(bit_value))
			{
				containers.emplace(bit_value, new Container<T>());
				signature.set(bit_value, true);
			}

			//Add component signature to entity.
			entity_allocator.get(entity).set(bit_value, true);

			//Add entity to relevant collections.
			for (std::pair<size_t, ISystem*> table : systems)
			{
				//Subset for entity with all it's components.
				if (table.first == entity_allocator.get(entity).get_decimal())
				{
					systems[entity_allocator.get(entity).get_decimal()]->entities.push_back(entity);
				}

				//Subset for entity with only the added component.
				else if (table.first == bit_to_decimal(bit_value))
				{
					systems[bit_to_decimal(bit_value)]->entities.push_back(entity);
				}
			}

			static_cast<Container<T>*>(containers[bit_value])->add_component(entity, component);
		}

		entity_t generate_entity() { return entity_allocator.allocate(); }
	};
}

#endif // !NP_ECS_CORE_H