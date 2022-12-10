#ifndef NP_ECS_STATIC_CORE_HPP
#define NP_ECS_STATIC_CORE_HPP

//Includes:
#include "../logic/system.h"
#include "../allocators/components.hpp"

namespace np_ecs
{
	struct BaseContainer { };

	template<typename T, size_t Size> class StaticContainer : BaseContainer
	{
	public:
		//Attributes:
		T components[Size];

		//Methods:
		void add_component(entity_t entity, T component)
		{
			components[entity] = component;
		}

		//Operators:
		T& operator[] (entity_t entity) { return components[entity]; }
	};

	template<unsigned int E_Limit, unsigned int C_Limit> class World
	{
	private:
		//Attributes:
		EntityAllocator entity_allocator;
		ComponentAllocator c_author;
		Bitmask<SignatureSize> signature;
		BaseContainer* containers[C_Limit];
		std::unordered_map<size_t, System*> systems;

	public:
		//Methods:
		template<typename T> bool assign_collection(System* system)
		{
			//Add system.
			size_t component_signature = bit_to_decimal(c_author.get_id<T>());
			system->local_world = this;
			systems.emplace(component_signature, system);

			//Add any entities with components to the system.
			for (int i = 0; i < entity_allocator.signatures.size(); i++)
			{
				if (entity_allocator.signatures[i] == component_signature)
				{
					systems[component_signature]->entities.push_back(i);
				}
				else if (entity_allocator.signatures[i].has_signature(component_signature))
				{
					systems[component_signature]->entities.push_back(i);
				}
			}

			return true;
		}

		template<typename T> T& get_component(entity_t entity)
		{
			assert(c_author.is_registered<T>() && entity < E_Limit);

			size_t index = c_author.get_id<T>();
			return (*static_cast<StaticContainer<T, E_Limit>*>(containers[index]))[entity];
		}

		template<typename T> void add_component(entity_t entity, T component)
		{
			assert( entity < E_Limit );

			size_t bit_value = c_author.get_id<T>();

			//Add component signature to entity.
			entity_allocator.get(entity).set(bit_value, true);

			//Add entity to relevant collections.
			for (std::pair<size_t, System*> table : systems)
			{
				if (table.first == entity_allocator.get(entity).get_decimal())
				{
					systems[entity_allocator.get(entity).get_decimal()]->entities.push_back(entity);
				}
				else if (table.first == bit_to_decimal(bit_value))
				{
					systems[bit_to_decimal(bit_value)]->entities.push_back(entity);
				}
			}

			static_cast<StaticContainer<T, E_Limit>*>(containers[bit_value])->add_component(entity, component);
		}

		entity_t generate_entity() { return entity_allocator.allocate(); }
	};

}

#endif // !NP_ECS_STATIC_CORE_HPP