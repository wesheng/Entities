#pragma once

#include "Entity.h"
#include <vector>
#include <unordered_map>

struct IComponentArray {
	virtual void Remove(unsigned int entity) {};
};

template<typename T>
class ComponentArray : public IComponentArray
{
public:
	void Insert(unsigned int entity, T component);
	T* Get(unsigned int entity);
	void Remove(unsigned int entity);
private:
	// std::vector<T> _components;
	std::unordered_map<unsigned int, T> _entities;
};

template<typename T>
void ComponentArray<T>::Insert(unsigned int entity, T component)
{
	// _entities[entity] = _components.size();
	_entities.emplace(entity, component);
	// _components.push_back(component);
}

template<typename T>
T* ComponentArray<T>::Get(unsigned int entity)
{
	auto it = _entities.find(entity);
	if (it != _entities.end())
	{
		return &(it->second);
	}
	return nullptr;
}

template<typename T>
inline void ComponentArray<T>::Remove(unsigned int entity)
{
	auto it = _entities.find(entity);
	if (it != _entities.end())
	{
		_entities.erase(entity);
	}
}
