#ifndef COMPONENT_H
#define COMPONENT_H
#define INVALID_COMPONENT -1
#define START -1
#define HASH_CODE(type) typeid(type).hash_code()
#define VALIDATE_COMPONENT(type) std::is_base_of<IComponent, type>::value

//Includes:
#include<bitset>
#include<vector>
#include<queue>
#include<map>

struct IComponent
{
private:
	//Attributes:
	unsigned int identification;

public:
	//Default constructor:
	IComponent(unsigned int id) : identification(id) { }

	//Methods:
	unsigned int get_identification() { return identification; };
	template<typename Type> static bool is_valid_component() { return std::is_base_of<IComponent, Type>::value; }
};


class ComponentAuthor
{
private:
	//Attributes:
	std::queue<unsigned int> free;
	std::map<unsigned int, unsigned int> registry;
	unsigned int current_id;

	//Methods:
	unsigned int next_identity()
	{
		if (free.empty()) return ++current_id;
		unsigned int id = free.front();
		free.pop();
		return id;
	}

public:
	//Constructor:
	ComponentAuthor() : current_id(START) { }

	//Methods:
	template<typename ComponentType> bool component_is_registered()
	{
		return registry.find(typeid(ComponentType).hash_code()) != registry.end();
	}

	template<typename ComponentType> unsigned int get_id()
	{
		if (!IComponent::is_valid_component<ComponentType>()) return INVALID_COMPONENT;

		//Type deriving IComponent has not been registered.
		if (registry.find(typeid(ComponentType).hash_code()) == registry.end())
		{
			registry.insert(std::pair<unsigned int, unsigned int>(typeid(ComponentType).hash_code(), next_identity()));
		}

		return registry[typeid(ComponentType).hash_code()];
	}

	template<typename ComponentType> void free_identity()
	{
		unsigned int hash = typeid(ComponentType).hash_code();

		if (!IComponent::is_valid_component<ComponentType>() || registry.find(hash) == registry.end()) return;

		free.push(registry[hash]);
		registry.erase(hash);
	}

	template<typename A, typename B> void swap_identities()
	{
		if (!IComponent::is_valid_component<A>() || !IComponent::is_valid_component<B>()) return;
		unsigned int a = typeid(A).hash_code(), b = typeid(B).hash_code();

		unsigned int b_value = registry[b];
		registry[b] = registry[a];
		registry[a] = b_value;
	}
};


#endif // !COMPONENT_H