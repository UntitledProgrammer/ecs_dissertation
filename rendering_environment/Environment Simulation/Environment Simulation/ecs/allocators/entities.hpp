#ifndef NP_ECS_ENTITIES_H
#define NP_ECS_ENTITIES_H

//Includes:
#include  <queue>
#include "../logic/bitmask.hpp"
#include "../include/type_definitions.h"

#define SignatureSize 10

namespace np_ecs
{
	class EntityAllocator
	{
	private:
		//Attributes:
		std::vector<Bitmask<SignatureSize>> signatures;
		std::queue<entity_t> available_entities;

	public:
		//Methods:
		bool validate(entity_t entity) { return entity < signatures.size(); }

		entity_t allocate()
		{
			if (!available_entities.empty())
			{
				entity_t entity = available_entities.front();
				available_entities.pop();
				return entity;
			}

			signatures.push_back(Bitmask<SignatureSize>());
			return signatures.size() - 1;
		}

		Bitmask<SignatureSize>& get(unsigned int entity)
		{
			assert(validate(entity));

			return signatures[entity];
		}

		void free(unsigned int entity)
		{
			assert(validate(entity));

			available_entities.push(entity);
			signatures[entity].reset();
		}

		//Friends:
		friend class World;
	};
}

#endif // !NP_ECS_ENTITIES_H