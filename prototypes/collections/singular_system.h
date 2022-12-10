#ifndef SINGULAR_SYSTEM_H
#define SINGULAR_SYSTEM_H

//Includes:
#ifndef SIMULATABLE_H
#include"simulatable.h"
#endif // !SIMULATABLE_H

#include<vector>
#include<bitset>

template<unsigned int ComponentLength, typename ComponentType>
class ISingularSystem : public ISimulatable
{
private:
	std::bitset<ComponentLength> desired_component;

public:
	//Constructor:
	ISingularSystem() { desired_component.set(GetID<ComponentType>(), true); }

	//Methods:
	virtual Result OnInitialise() = 0;
	virtual Result OnUpdate() = 0;
	virtual Result OnTerminate() = 0;

	std::bitset<ComponentLength> get_component_id() { return desired_component; }
};

#endif // !SINGULAR_SYSTEM_H