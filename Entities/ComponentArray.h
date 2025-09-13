#pragma once

#include "Entity.h"
#include <vector>
#include <unordered_map>

struct IComponentArray {
};

template<typename T>
class ComponentArray : public IComponentArray
{
public:
	void Insert(size_t e, T component);
	T* Get(size_t entity);
	void Remove(size_t entity);
private:
	std::vector<T> _components;
	std::unordered_map<size_t, size_t> _entities;
};

template<typename T>
void ComponentArray<T>::Insert(size_t e, T component)
{
	_entities[e] = _components.size();
	_components.push_back(component);
}

template<typename T>
T* ComponentArray<T>::Get(size_t entity)
{
	auto it = _entities.find(entity);
	if (it != _entities.end())
	{
		return &(_components[it->second]);
	}
	return nullptr;
}

template<typename T>
inline void ComponentArray<T>::Remove(size_t entity)
{
	auto it = _entities.find(entity);
	if (it != _entities.end())
	{
		_components.erase(it);
	}
}
