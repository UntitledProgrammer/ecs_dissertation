#ifndef NP_ECS_COMPONENTS_HPP
#define NP_ECS_COMPONENTS_HPP

//Includes:
#include "../allocators/components.hpp"

namespace np_ecs
{
	struct Container{ };

	template<typename T> struct ComponentContainer : Container
	{
		std::vector<T> components;
	};

	class ComponentStorage
	{
	public:
		//Attributes:
		std::unordered_map<hash_t, Container*> containers;

		//Methods:
		template<typename T> void add_component(entity_t entity, T component)
		{
			assert(derives_component<T>());

			//Register component.
			if (containers.find(HASH_CODE(T)) == containers.end())
			{
				containers.emplace(HASH_CODE(T), new ComponentContainer<T>());
			}

			containers[HASH_CODE(T)]->components.push_back()
		}
	};
}

#endif // ! NP_ECS_COMPONENTS_HPP