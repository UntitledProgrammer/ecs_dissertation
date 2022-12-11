#ifndef WORLD_H
#define WORLD_H

//Includes:
#include "time.hpp"
#include <SFML/Graphics.hpp>

namespace np_application
{
	namespace settings
	{
		const unsigned int WIDTH = 800;
		const unsigned int HEIGHT = 600;
	}

	struct World
	{
	public:
		sf::RenderWindow window;

		World() { window.create(sf::VideoMode(settings::WIDTH, settings::HEIGHT), "SFML Works!"); }
	};

	static World* get_world()
	{
		static World* world;
		return world;
	}
}

#endif //!WORLD_H