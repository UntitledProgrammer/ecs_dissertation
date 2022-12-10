#ifndef NP_ECS_RENDER_SYSTEM_HPP
#define NP_ECS_RENDER_SYSTEM_HPP

//Includes:
#include "../components/sprite.h"
#include "../../ecs/include/core.h"
#include <unordered_map>
#include <SFML/Graphics.hpp>

namespace np_app
{
	class RenderSystem : public np_ecs::ISystem
	{
	private:
		//Attributes:
		std::unordered_map<std::string, sf::Texture*> textures;

	public:
		//Attributes:
		sf::RenderTarget* target;

		//Methods:
		Sprite get_sprite(std::string filepath, bool smooth = false, sf::IntRect boundary = sf::IntRect())
		{
			if (textures.find(filepath) == textures.end())
			{
				sf::Texture* texture = new sf::Texture();
				assert(texture->loadFromFile(filepath, boundary));

				textures.emplace(filepath, texture);
			}

			Sprite sprite;
			sprite.texture = textures[filepath];
			sprite.texture->setSmooth(smooth);
			sprite.sprite.setTexture(*sprite.texture);

			return sprite;
		}

		np_ecs::Result update()
		{
			if (world() == nullptr) return np_ecs::Result::Failure;

			for (int i = 0; i < entities.size(); i++)
			{
				target->draw(world()->get_component<Sprite>(entities[i]).sprite);
			}

			return np_ecs::Result::Sucess;
		}

		np_ecs::Result start()
		{
			return np_ecs::Result::Sucess;
		}
	};
}

#endif // !NP_ECS_RENDER_SYSTEM_HPP