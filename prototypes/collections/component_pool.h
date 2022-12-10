#ifndef COMPONENT_POOL_H
#define COMPONENT_POOL_H

//Includes:
#include<map>
#include<vector>
#include"component.h"
#include"simulatable.h"

struct ComponentPool
{
private:
	//Attributes:
	std::vector<std::shared_ptr<IComponent>> components;
	std::map<unsigned int, unsigned int> entity_to_index;

public:
	//Constructor:
	ComponentPool() { }

	//Methods:
	template<typename T> void add_component(unsigned int entity, std::shared_ptr<T> component)
	{
		//Entity already exists...
		if (entity_to_index.find(entity) != entity_to_index.end()) return;
		entity_to_index.emplace(std::pair<unsigned int, unsigned int>(entity, components.size()));
		components.push_back(component);
	}

	template<typename T> std::shared_ptr<IComponent> get_component(unsigned int entity)
	{
		return components[entity_to_index[entity]];
	}
};

template<unsigned int SignatureSize> struct Collection
{
public:
	std::vector<unsigned int> entities;
private:
	std::bitset<SignatureSize> signature;
};


class Administrator
{
private:
	//Attributes:
	std::map<unsigned int, ComponentPool> pools;
	std::map<unsigned int, Collection<10>> collections; // Don't hardcode this...
	ComponentAuthor component_author;
	EntityAuthor<10> entity_author; // Don't hardcode this...

public:
	//Methods:
	template<typename T> void add_component(unsigned int entity, T component)
	{
		assert(VALIDATE_COMPONENT(T)); //T does not derive IComponent.

		unsigned int hash = HASH_CODE(T);

		//Component pool does not currently exist.
		if (pools.find(hash) == pools.end())
		{
			pools.emplace(hash, ComponentPool());
		}

		entity_author.get(entity).set(component_author.get_id<T>(), true);
		unsigned int value = entity_author.get(entity).count();

		//Add entity to collection.
		if (collections.find(value) != collections.end()) { collections[value].entities.push_back(entity); }

		//Add component to pool.
		pools[hash].add_component<T>(entity, std::make_shared<T>(component));
	}

	unsigned int allocate_entity()
	{
		return entity_author.generate();
	}

	template<typename T> Collection<10>& get_collection()
	{
		assert(VALIDATE_COMPONENT(T)); //T does not derive IComponent.

		std::bitset<10> bit;
		bit.set(component_author.get_id<T>(), true);

		//Collection has already been created.
		if (collections.find(bit.count()) != collections.end()) { return collections[bit.count()]; }

		collections.emplace(bit.count(), Collection<10>());
		return collections[bit.count()];
	}

	template<typename T> std::shared_ptr<IComponent> get_component(unsigned int entity)
	{
		assert(VALIDATE_COMPONENT(T));

		return pools[HASH_CODE(T)].get_component<T>(entity);
	}
};



#endif // !COMPONENT_POOL_H