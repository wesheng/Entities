#pragma once

#include "Entity.h"
#include <initializer_list>
#include <vector>
#include <queue>
#include <tuple>
#include <unordered_map>
#include <typeindex>
#include "ComponentArray.h"
#include <memory>
#include "Query.h"
#include <functional>
#include <bitset>
#include "System.h"

/// <summary>
/// THe max number of components.
/// </summary>
const unsigned int MAX_COMPONENTS = 100;

static unsigned int Component_Count = 0;

/// <summary>
/// Assigns and retrieves a component identifier.
/// </summary>
/// <typeparam name="T">The component to obtain an identifier from.</typeparam>
/// <returns>The identifier.</returns>
template<typename T>
unsigned int GetComponentId() {
	static unsigned int id = Component_Count++;
	return id;
}

class World
{
public:
	/// <summary>
	/// Creates an entity.
	/// </summary>
	/// <typeparam name="...T">A list of components formatted as a structure.</typeparam>
	/// <param name="...components">The component data.</param>
	/// <returns>A new entity.</returns>
	/// <remarks>
	/// Entities only contain identifiers. It is not aware of any components that it has.
	/// </remarks>
	template<typename... T>
	Entity Create(T... components);
	/// <summary>
	/// Removes an entity (and associated components) from the world. Does nothing if the entity does not exist.
	/// </summary>
	/// <param name="entity">The entity to remove.</param>
	void Remove(Entity entity);
	/// <summary>
	/// Adds or modifies component(s) to an entity.
	/// </summary>
	/// <typeparam name="...T">A list of component types to add.</typeparam>
	/// <param name="entity">The entity to modify.</param>
	/// <param name="...components">The list of components to add.</param>
	template<typename... T>
	void Attach(Entity entity, T... components);
	/// <summary>
	/// Removes a component from an entity.
	/// </summary>
	/// <typeparam name="T">The component type to remove.</typeparam>
	/// <param name="entity">The entity to remove the component from.</param>
	template<typename T>
	void Detach(Entity entity);

	template<typename... T>
	void System(void(*callback)(World* world, Entity entity, T... components));

	/// <summary>
	/// Retrieves the component value from an Entity. To update the component use Attach().
	/// </summary>
	/// <typeparam name="T">The component type to retrieve.</typeparam>
	/// <param name="entity">The entity to retrieve the component for.</param>
	/// <returns></returns>
	template<typename T>
	T GetComponent(Entity entity);

private:
	template<typename ... Targs>
	std::tuple<Targs* ...> GetComponents(Entity e);

	/// <summary>
	/// Retrieves the bitmask from specified components.
	/// </summary>
	/// <typeparam name="...Targs"></typeparam>
	/// <returns></returns>
	template<typename ... Targs>
	std::bitset<MAX_COMPONENTS> GetMask();

	/// <summary>
	/// Recursively adds components to an entity.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <typeparam name="...Targs"></typeparam>
	/// <param name="entity"></param>
	/// <param name="component"></param>
	/// <param name="...components"></param>
	template<typename T, typename... Targs>
	void TAttach(Entity entity, T component, Targs... components);
	// Basecase of above function. Does nothing.
	void TAttach(Entity entity);

private:
	/// <summary>
	/// The list of entities.
	/// </summary>
	std::vector<unsigned int> _entities;
	/// <summary>
	/// Manages removed entities. This enables quick retrieval of freed _entities slots.
	/// </summary>
	std::queue<unsigned int> _freeList;
	/// <summary>
	/// Used to keep track of the components that an entity has, for quick lookup.
	/// </summary>
	std::vector<std::bitset<MAX_COMPONENTS>> _componentMasks;

	std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> _components;
};

template<typename... T>
inline Entity World::Create(T... components)
{
	unsigned int index = (unsigned int)_entities.size();

	// start counting generation from 1. 0 represents an invalid entity.
	unsigned int generation = 1;
	if (_freeList.empty())
	{
		index = (unsigned int)_entities.size();
		_entities.push_back(generation);
		_componentMasks.push_back(0);
	}
	else
	{
		index = _freeList.front();
		_freeList.pop();
		generation = ++_entities[index];
	}

	Entity e;
	e.ID = index;
	e.Generation = generation;

	Attach(e, components...);

	return e;
}

template<typename... T>
inline void World::Attach(Entity e, T... components)
{
	TAttach(e, components...);
}

template<typename T>
inline void World::Detach(Entity e)
{
	int generation = _entities[e.ID];

	if (generation == e.Generation)
	{
		std::type_index id = std::type_index(typeid(T));
		auto it = _components.find(id);
		if (it != _components.end())
		{
			ComponentArray<T>* array = static_cast<ComponentArray<T>*>(_components[id].get());
			array->Remove(e.ID);
		}
	}
}

template<typename ...T>
inline void World::System(void(*callback)(World* world, Entity e, T...components))
{
	std::bitset<MAX_COMPONENTS> mask = GetMask<T...>();

	for (uint32_t entityId = 0; entityId < _componentMasks.size(); entityId++)
	{
		if (_entities[entityId] > 0)
		{
			if ((_componentMasks[entityId] & mask) == mask) {
				Entity e;
				e.ID = entityId;
				e.Generation = _entities[entityId];

				callback(this, e, (GetComponent<T>(e))...);
			}
		}
	}
}

template<typename T>
inline T World::GetComponent(Entity e)
{
	int generation = _entities[e.ID];

	std::type_index id = std::type_index(typeid(T));
	auto it = _components.find(id);
	if (generation == e.Generation && it != _components.end())
	{
		ComponentArray<T>* array = static_cast<ComponentArray<T>*>(_components[id].get());
		return *(array->Get(e.ID));
	}
	return T();
}

template<typename ...Targs>
inline std::bitset<MAX_COMPONENTS> World::GetMask()
{
	std::bitset<MAX_COMPONENTS> mask;

	(mask.set(GetComponentId<Targs>()), ...);

	return mask;
}

template<typename T, typename ...Targs>
inline void World::TAttach(Entity e, T component, Targs ...components)
{
	int generation = _entities[e.ID];
	if (e.Generation == generation)
	{
		std::type_index id = std::type_index(typeid(T));
		auto it = _components.find(id);
		if (it == _components.end())
		{
			_components[id] = std::make_unique<ComponentArray<T>>();
		}

		ComponentArray<T>* array = static_cast<ComponentArray<T>*>(_components[id].get());
		array->InsertOrReplace(e.ID, component);

		unsigned int componentId = GetComponentId<T>();
		_componentMasks[e.ID].set(componentId);

		TAttach(e, components...);
	}
}

inline void World::Remove(Entity e)
{
	int generation = _entities[e.ID];
	if (generation == e.Generation)
	{
		std::bitset<MAX_COMPONENTS> componentMask = _componentMasks[e.ID];
		for (auto& it : _components)
		{
			IComponentArray* componentArray = it.second.get();
			componentArray->Remove(e.ID);
		}
		_entities[e.ID] = 0;
		_freeList.emplace(e.ID);
	}
}

inline void World::TAttach(Entity e)
{
	return;
}