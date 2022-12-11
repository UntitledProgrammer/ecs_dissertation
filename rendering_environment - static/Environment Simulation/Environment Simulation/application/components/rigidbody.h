#ifndef NP_APP_RIGIDBODY_HPP
#define NP_APP_RIGIDBODY_HPP

//Includes:
#include<SFML/Graphics.hpp>

namespace np_app
{
	struct Rigidbody
	{
	public:
		//Attributes:
		double mass;
		Sprite* sprite;

		//Constructor:
		Rigidbody() : mass(0.00f), sprite(nullptr) {}
		Rigidbody(double mass, Sprite* sprite) : mass(mass), sprite(sprite) {}
	};
}

#endif // !NP_APP_RIGIDBODY_HPP