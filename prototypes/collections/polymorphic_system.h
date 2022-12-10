//Includes:
#include<vector>
#include<map>
#include<bitset>
#include <stdio.h>
#include"simulatable.h"

//Defines:
#define DESIRED_SIZE 10

template<unsigned int Size, typename... Args> class ISystem : public ISimulatable
{
private:
	//Attributes:
	std::bitset<Size> desired_components;

	//Methods:
	template<typename Type> void Assign()
	{
		//Console information.
		std::printf("Adding %s (%u).", typeid(Type).name(), GetID<Type>());
		std::cout << std::endl;

		//Logic.
		desired_components.set(GetID<Type>(), true);
	}

public:
	//Constructor:
	ISystem()
	{
		int apply_signature[] = { 0, (Assign<Args>(), 0)... };
		(void)apply_signature;
	}

	std::bitset<Size> GetDesiredComponents() { return desired_components; }

	//Abstract methods:
	virtual Result OnInitialise() = 0;
	virtual Result OnTerminate() = 0;
	virtual Result OnUpdate() = 0;
};