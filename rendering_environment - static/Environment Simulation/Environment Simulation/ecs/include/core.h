#ifndef NP_ECS_CORE_H
#define NP_ECS_CORE_H

//Includes:
#include <unordered_map>
#include "../allocators/components.hpp"
#include "../logic/system.h"
#include "../storage/components.hpp"

namespace np_ecs
{
	struct BaseContainer { };

	template<typename T> struct Container : public BaseContainer
	{
	public:
		//Attributes:
		std::vector<T> components;

		//Constructor:
		Container() { }
		Container(unsigned int size) { components.resize(size); }

		//Methods:
		void add_component(entity_t entity, T component)
		{
			assert(entity < components.size());
			components[entity] = component;
		}

		//Operators:
		T& operator[] (entity_t entity) { assert(entity < components.size()); return components[entity]; }
	};

	class World
	{
	private:
		//Attributes:
		unsigned int entity_max, component_max;
		EntityAllocator entity_allocator;
		ComponentAllocator c_author;
		Bitmask<SignatureSize> signature;
		std::vector<BaseContainer*> containers;
		std::unordered_map<size_t, ISystem*> systems;

	public:
		//Constructors:
		World() { }
		World(unsigned int max_components, unsigned int max_entities) : entity_max(max_entities), component_max(max_components) { containers.resize(max_components); }


		//Methods:
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
			size_t flag = get_bit_flag<T>();
			return (*static_cast<Container<T>*>(containers[flag]))[entity];
		}


		template<typename T> T& get_component(entity_t entity, index_t bit_flag)
		{
			return (*static_cast<Container<T>*>(containers[bit_flag]))[entity];
		}


		template<typename T> void add_component(entity_t entity, T component)
		{
			assert(entity_allocator.validate(entity));

			size_t bit_flag = get_bit_flag<T>();

			//Register component.
			if (!signature.has_bit(bit_flag))
			{
				containers[bit_flag] = new Container<T>(component_max);
				signature.set(bit_flag, true);
			}

			//Add component signature to entity.
			entity_allocator.get(entity).set(bit_flag, true);

			//Add entity to relevant collections.
			for (std::pair<size_t, ISystem*> table : systems)
			{
				//Subset for entity with all it's components.
				if (table.first == entity_allocator.get(entity).get_decimal())
				{
					systems[entity_allocator.get(entity).get_decimal()]->entities.push_back(entity);
				}

				//Subset for entity with only the added component.
				else if (table.first == bit_to_decimal(bit_flag))
				{
					systems[bit_to_decimal(bit_flag)]->entities.push_back(entity);
				}
			}

			static_cast<Container<T>*>(containers[bit_flag])->add_component(entity, component);
		}

		entity_t generate_entity() { return entity_allocator.allocate(); }
	};
}

#endif // !NP_ECS_CORE_H