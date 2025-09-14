#pragma once

#include "Entity.h"
#include <vector>
#include <unordered_map>

/// <summary>
/// Base class that stores a set of components.
/// </summary>
struct IComponentArray {
	/// <summary>
	/// Remove a component assigned to an entity ID.
	/// </summary>
	/// <param name="entity">The entity ID</param>
	virtual void Remove(unsigned int entity) {};
};

template<typename T>
class ComponentArray : public IComponentArray
{
public:
	/// <summary>
	/// Inserts a component and the entity that is associated with it.
	/// </summary>
	/// <param name="entity">The entity ID</param>
	/// <param name="component">The component to add.</param>
	void Insert(unsigned int entity, T component);
	/// <summary>
	/// Inserts or updates a component with the entity that is associated with it.
	/// </summary>
	/// <param name="entity">The entity ID</param>
	/// <param name="component">The component to add or replace</param>
	void InsertOrReplace(unsigned int entity, T component);
	/// <summary>
	/// Retrieves a pointer to a stored component.
	/// </summary>
	/// <param name="entity">The entity ID</param>
	/// <returns>The component reference.</returns>
	T* Get(unsigned int entity);
	/// <summary>
	/// Removes a component associated with the specified entity ID.
	/// </summary>
	/// <param name="entity"The entity ID></param>
	void Remove(unsigned int entity);
private:
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
