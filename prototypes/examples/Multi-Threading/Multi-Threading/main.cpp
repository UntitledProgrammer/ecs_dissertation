#include <iostream>
#include<thread>
#include<string>
#include<chrono>
#include<vector>

#define MICRO_TO_SECOND (int)1e+6

class Component
{
private:
	//Attributes:
	unsigned int entityID;

public:
	//Methods:
	virtual void OnInitialise() = 0;
};


class Position : public Component
{
public:
	//Attributes:
	float horizontal;
	float vertical;

	//Methods:
	void OnInitialise()
	{
		horizontal = 0;
		vertical = 0;
	}
};

class Simulated
{
public:
	//Methods:
	virtual void Awake() = 0;
	virtual void OnFrame() = 0;
};

template<typename ComponentType>
class Processor: public Simulated
{
protected:
	//Attributes:
	std::vector<ComponentType*> components;

public:
	//Methods:
	void Add(ComponentType* component)
	{
		components.push_back(component);
	}

	void Awake() = 0;
	void OnFrame() = 0;
};

class PositionProcessor : public Processor<Position>
{
public:
	//Methods:
	void Awake()
	{
		for (int i = 0; i < components.size(); i++)
		{
			components[i]->OnInitialise();
		}
	}

	void OnFrame()
	{
		for (int i = 0; i < components.size(); i++)
		{
			components[i]->horizontal += 4;
			components[i]->vertical += 2;

			std::cout << "Horizontal: " << components[i]->horizontal << ", Vertical: " << components[i]->vertical << std::endl;
		}
	}
};


#include<unordered_map>

class System
{
private:
	//Attributes:
	std::unordered_map<unsigned int, Simulated*> systems;

public:
	//Methods:
	void Process()
	{
		for (auto simulated : systems)
		{
			simulated.second->OnFrame();
		}
	}

	template<typename ComponentType> void Add(Processor<ComponentType>* system)
	{
		systems[typeid(ComponentType).hash_code()] = system;
	}
};


int main()
{
	System sys = System();
	PositionProcessor processor = PositionProcessor();
	sys.Add<Position>(&processor);
	Position position = Position();
	processor.Add(&position);

	while (true)
	{
		sys.Process();
	}

	return 0;
}