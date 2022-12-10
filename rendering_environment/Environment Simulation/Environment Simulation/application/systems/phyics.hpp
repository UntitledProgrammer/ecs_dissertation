#ifndef NP_APP_PHYSICS_HPP
#define NP_APP_PHYSICS_APP

//Includes:
#include "../../ecs/include/core.h"
#include "../components/rigidbody.h"
#include <iostream>

namespace np_app
{

	class Physics : public np_ecs::ISystem
	{
		double distance(sf::Vector2f a, sf::Vector2f b) { return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)); }

		np_ecs::Result update()
		{
			if (world() == nullptr) return np_ecs::Result::Failure;


			for (int i = 0; i < entities.size(); i++)
			{
				Rigidbody& focus = world()->get_component<Rigidbody>(entities[i]);

				for (int j = 0; j < entities.size(); i++)
				{

				}
			}

			return np_ecs::Result::Sucess;
		}

		np_ecs::Result start()
		{
			return np_ecs::Result::Sucess;
		}
	};

}

#endif \\!NP_APP_PHYSICS_APP