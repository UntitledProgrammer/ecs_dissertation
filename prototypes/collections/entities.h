#ifndef ENTITY_H
#define ENTITY_H

//Includes:
#include<bitset>
#include<vector>
#include<atomic>
#include<optional>
#include<queue>

//Defines:
#define MINIMUM 0
#define EMPTY 0
#define INVALID_ENTITY_MASK nullptr

template<unsigned int BitLength> class EntityAuthor
{
private:
	//Attributes:
	std::vector<std::bitset<BitLength>> entities;
	std::queue<unsigned int> available_entities;

public:
	//Methods:
	bool validate(unsigned int entity) { return (entity >= MINIMUM && entity < entities.size()); }

	inline unsigned int generate()
	{
		if (available_entities.size() > EMPTY)
		{
			unsigned int entity = available_entities.front();
			available_entities.pop();
			return entity;
		}

		entities.push_back(std::bitset<BitLength>());
		return entities.size()-1;
	}

	std::bitset<BitLength>& get(unsigned int entity)
	{
		//if (!validate(entity)) return 0;
		return entities[entity];
	}

	bool free(unsigned int entity)
	{
		if (!validate(entity)) return false;

		available_entities.push(entity);
		entities[entity]->reset();
	}
};

#endif // !ENTITY_H