// Environment Simulation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Constraints.h"
#include "application/systems/rendering.hpp"
#include "ecs/include/core.h"
#include "profiler/src/Timer.h"
#include "ecs/storage/subset.hpp"
#include "application/systems/phyics.hpp"




int main()
{
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
		world.add_component<Sprite>(entity, sprite);
		world.add_component<np_app::Rigidbody>(entity, np_app::Rigidbody(1.00f, &sprite));
	}

	//Window loop:
	while (window.isOpen())
	{
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
	}

}