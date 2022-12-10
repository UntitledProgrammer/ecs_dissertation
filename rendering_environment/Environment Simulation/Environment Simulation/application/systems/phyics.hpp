#ifndef NP_APP_PHYSICS_HPP
#define NP_APP_PHYSICS_APP

//Includes:
#include "../../ecs/include/core.h"
#include "../components/rigidbody.h"
#include <iostream>

namespace np_app
{

	//Constants:
	const double GRAVITATIONAL_CONSTANT = 6.673e-11;

	class Physics : public np_ecs::ISystem
	{
		double distance(sf::Vector2f a, sf::Vector2f b) { return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)); }

		np_ecs::Result update()
		{
			if (world() == nullptr) return np_ecs::Result::Failure;


			for (int i = 0; i < entities.size(); i++)
			{
				Rigidbody& focus = world()->get_component<Rigidbody>(entities[i]);

				for (int j = 0; j < entities.size(); j++)
				{
					if (j == i) continue; //We don't want to apply a gravitational force against itself.

					double d = distance(world()->get_component<Sprite>(entities[i]).sprite.getPosition(), world()->get_component<Sprite>(entities[j]).sprite.getPosition());

					std::cout << "Distance: " << d << std::endl;

					double gravitational_force = GRAVITATIONAL_CONSTANT * (world()->get_component<Rigidbody>(entities[j]).mass * focus.mass) / d;

					std::cout << "Force: " << gravitational_force << std::endl;

					sf::Vector2f direction = world()->get_component<Sprite>(entities[j]).sprite.getPosition() - world()->get_component<Sprite>(entities[i]).sprite.getPosition();
					direction.x *= gravitational_force;
					direction.y *= gravitational_force;
					world()->get_component<Sprite>(entities[i]).sprite.setPosition(world()->get_component<Sprite>(entities[i]).sprite.getPosition() + direction);
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