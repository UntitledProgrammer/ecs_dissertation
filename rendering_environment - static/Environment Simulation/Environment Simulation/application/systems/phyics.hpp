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
		//Constants:
		const unsigned int rigidbody_flag = np_ecs::get_bit_flag<Rigidbody>();
		const unsigned int sprite_flag = np_ecs::get_bit_flag<Sprite>();

		//Methods:
		double distance(sf::Vector2f a, sf::Vector2f b) { return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)); }

		np_ecs::Result update()
		{
			if (world() == nullptr) return np_ecs::Result::Failure;


			for (int i = 0; i < entities.size(); i++)
			{
				Rigidbody& focus = world()->get_component<Rigidbody>(entities[i], rigidbody_flag);
				Sprite& sprite_focus = world()->get_component<Sprite>(entities[i], sprite_flag);

				for (int j = 0; j < entities.size(); j++)
				{
					if (j == i) continue; //We don't want to apply a gravitational force against itself.

					Rigidbody& external = world()->get_component<Rigidbody>(entities[j], rigidbody_flag);
					Sprite& sprite_external = world()->get_component<Sprite>(entities[j], sprite_flag);

					double d = distance(sprite_focus.sprite.getPosition(), sprite_external.sprite.getPosition());

					double gravitational_force = GRAVITATIONAL_CONSTANT * (external.mass * focus.mass) / d;


					sf::Vector2f direction = sprite_external.sprite.getPosition() - sprite_focus.sprite.getPosition();

					//SFML has no Vector * Float operators...
					direction.x *= gravitational_force;
					direction.y *= gravitational_force;

					sprite_focus.sprite.setPosition(sprite_focus.sprite.getPosition() + direction);
				}
			}

			return np_ecs::Result::Sucess;
		}

		np_ecs::Result start()
		{
			if (world() == nullptr) return np_ecs::Result::Failure;

			return np_ecs::Result::Sucess;
		}
	};

}

#endif \\!NP_APP_PHYSICS_APP