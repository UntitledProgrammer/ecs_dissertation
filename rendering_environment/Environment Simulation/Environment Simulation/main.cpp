// Environment Simulation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdlib.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Constraints.h"
#include "application/systems/rendering.hpp"
#include "ecs/include/core.h"
#include "profiler/src/Timer.h"
#include "ecs/storage/subset.hpp"
#include "application/systems/phyics.hpp"
#include "application/world.h"




int main()
{
	np_app::Timer timer;
	std::srand(timer.elapsed_milliseconds());

	//Create window:
	sf::RenderWindow window(sf::VideoMode(sim::width, sim::height), "SFML Works!");


	np_ecs::World world;
	np_app::RenderSystem* render_system = (np_app::RenderSystem*) world.register_system<np_app::RenderSystem>(world.combine_masks<Sprite>());
	np_app::Physics* physics_system = (np_app::Physics*)world.register_system<np_app::Physics>(world.combine_masks<Sprite, np_app::Rigidbody>());
	render_system->target = &window;

	for (int i = 0; i < 10; i++)
	{
		np_ecs::entity_t entity = world.generate_entity();

		Sprite sprite = render_system->get_sprite("asteroid.png");
		sprite.sprite.setScale(sf::Vector2f(0.25f, 0.25f));
		sprite.sprite.setPosition(sf::Vector2f(std::rand() % 500 + 0, std::rand() % 500 + 0));
		world.add_component<Sprite>(entity, sprite);
		world.add_component<np_app::Rigidbody>(entity, np_app::Rigidbody(1000000.00f, &sprite));
	}

	//Window loop:
	while (window.isOpen())
	{
		timer.restart();

		//Poll events:
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
		}

		//Render frame:
		window.clear();

		world.update();

		window.display();

		std::cout << timer.elapsed_seconds() << std::endl;
	}

}