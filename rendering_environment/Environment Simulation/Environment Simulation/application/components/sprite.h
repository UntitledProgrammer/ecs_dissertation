#include<SFML/Graphics.hpp>
#include<assert.h>

struct Sprite
{
public:
	sf::Texture* texture;
	sf::Sprite sprite;

public:
	Sprite() : texture(nullptr), sprite(sf::Sprite()) { }
	friend class RenderSystem;
	friend Sprite create_sprite(const char*, bool, sf::IntRect);
	friend void draw_sprite(sf::RenderWindow&, Sprite&);
};

Sprite create_sprite(const char* filepath, bool smooth = false, sf::IntRect boundary = sf::IntRect())
{
	Sprite sprite;
	sprite.texture = new sf::Texture();

	assert(sprite.texture->loadFromFile(filepath, boundary));

	sprite.texture->setSmooth(smooth);
	sprite.sprite.setTexture(*sprite.texture);

	return sprite;
}

void draw_sprite(sf::RenderWindow& window, Sprite& sprite)
{
	window.draw(sprite.sprite);
}