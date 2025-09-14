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
	void InsertOrReplace(unsigned int entity, T component);
	T* Get(unsigned int entity);
	void Remove(unsigned int entity);
private:
	// std::vector<T> _components;
	std::unordered_map<unsigned int, T> _entities;
};

template<typename T>
void ComponentArray<T>::Insert(unsigned int entity, T component)
{
	_entities.emplace(entity, component);
}

template<typename T>
void ComponentArray<T>::InsertOrReplace(unsigned int entity, T component)
{
	auto it = _entities.find(entity);
	if (it != _entities.end())
	{
		_entities.erase(it);
	}
	_entities.emplace(entity, component);
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
