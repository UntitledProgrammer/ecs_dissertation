#ifndef NP_ECS_SYSTEM_H
#define NP_ECS_SYSTEM_H

//Includes:
#include "../include/type_definitions.h"

//Defines:
#define validate_system(type) std::is_base_of<ISystem, type>::value 

namespace np_ecs
{
	//Forward declaration.
	class World;

	//Enums.
	enum class Result { Sucess, Failure, Unexpected};

	class ISystem
	{
	public:
		//Attributes:
		std::vector<entity_t> entities;
		World* local_world;

	public:
		//Constructors:
		ISystem() { }
		ISystem(World* world) : local_world(world), entities(std::vector<entity_t>()) { }

		//Methods:
		World* world() { return local_world; }

		//Abstract methods:
		virtual Result update() = 0;
		virtual Result start() = 0;

		//Friends:
		friend class World;
	};
}

#endif // !NP_ECS_SYSTEM_H