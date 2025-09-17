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
#include "Utilities.h"

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

	/// <summary>
	/// Creates a system using the passed callback function. Note that components are readonly. Use Attach() to update.
	/// </summary>
	/// <typeparam name="...T">The components to modify.</typeparam>
	/// <param name="callback">The callback function.</param>
	template<typename... T>
	void System(void(*callback)(World& world, Entity entity, T... components));

	template<typename...T>
	void SystemQ(void(*callback)(World& world, Query<T...> query));

	template<typename ...T, typename...U>
	void SystemQ(void(*callback)(World& world, Query<T...> query, Without<U...> queryWithout));

	/// <summary>
	/// Retrieves the component value from an Entity. To update the component use Attach().
	/// </summary>
	/// <typeparam name="T">The component type to retrieve.</typeparam>
	/// <param name="entity">The entity to retrieve the component for.</param>
	/// <returns></returns>
	template<typename T>
	T GetComponent(Entity entity);

	/// <summary>
	/// Checks to see if a component is attached to an Entity.
	/// </summary>
	/// <typeparam name="T">The component type to check.</typeparam>
	/// <param name="entity">The entity to check the component for.</param>
	/// <returns></returns>
	template<typename T>
	bool HasComponent(Entity entity);

private:
	template<typename ... Targs>
	std::tuple<Targs* ...> GetComponents(Entity e);

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
	struct CreatedEntity {
		unsigned int Generation = 0;
		bool Instantiated = false;
	};

	/// <summary>
	/// The list of entities.
	/// </summary>
	std::vector<CreatedEntity> _entities;
	/// <summary>
	/// Manages removed entities. This enables quick retrieval of freed _entities slots.
	/// </summary>
	std::queue<unsigned int> _freeList;
	/// <summary>
	/// Used to keep track of the components that an entity has for quick lookup.
	/// </summary>
	std::vector<std::bitset<MAX_COMPONENTS>> _componentMasks;
	/// <summary>
	/// Stores all components.
	/// </summary>
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

		CreatedEntity e;
		e.Generation = generation;
		e.Instantiated = true;

		_entities.push_back(e);
		_componentMasks.push_back(0);
	}
	else
	{
		index = _freeList.front();
		_freeList.pop();
		_entities[index].Instantiated = true;
		generation = ++_entities[index].Generation;
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
	int generation = _entities[e.ID].Generation;

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
inline void World::System(void(*callback)(World& world, Entity e, T...components))
{
	std::bitset<MAX_COMPONENTS> mask = GetMask<T...>();

	for (uint32_t entityId = 0; entityId < _componentMasks.size(); entityId++)
	{
		if (_entities[entityId].Instantiated)
		{
			std::bitset<MAX_COMPONENTS> entityMask = _componentMasks[entityId];
			if ((entityMask & mask) == mask)  {
				Entity e;
				e.ID = entityId;
				e.Generation = _entities[entityId].Generation;

				callback(*this, e, (GetComponent<T>(e))...);
			}
		}
	}
}

template<typename...T>
inline void World::SystemQ(void(*callback)(World& world, Query<T...> query))
{
	std::bitset<MAX_COMPONENTS> mask = GetMask<T...>();
	for (uint32_t entityId = 0; entityId < _componentMasks.size(); entityId++)
	{
		if (_entities[entityId].Instantiated)
		{
			std::bitset<MAX_COMPONENTS> entityMask = _componentMasks[entityId];
			if ((entityMask & mask) == mask) {
				Entity e;
				e.ID = entityId;
				e.Generation = _entities[entityId].Generation;

				Query<T...> q;
				q.Entity = e;
				q.Components = std::make_tuple(GetComponent<T>(e)...);
				callback(*this, q);
			}
		}
	}
}

template<typename ...T, typename...U>
inline void World::SystemQ(void(*callback)(World& world, Query<T...> query, Without<U...> queryWithout))
{
	std::bitset<MAX_COMPONENTS> mask = GetMask<T...>();
	std::bitset<MAX_COMPONENTS> ignoreMask = GetMask<U...>();

	std::bitset<MAX_COMPONENTS> targetMask = mask & (~ignoreMask);
	std::bitset<MAX_COMPONENTS> testMask = mask | ignoreMask;

	for (uint32_t entityId = 0; entityId < _componentMasks.size(); entityId++)
	{
		if (_entities[entityId].Instantiated)
		{
			std::bitset<MAX_COMPONENTS> entityMask = _componentMasks[entityId];
			if ((entityMask & testMask) == targetMask) {
				Entity e;
				e.ID = entityId;
				e.Generation = _entities[entityId].Generation;

				Query<T...> q;
				q.Entity = e;
				q.Components = std::make_tuple(GetComponent<T>(e)...);
				Without<U...> qWithout;
				callback(*this, q, qWithout);
			}
		}
	}
}

template<typename T>
inline T World::GetComponent(Entity e)
{
	int generation = _entities[e.ID].Generation;

	std::type_index id = std::type_index(typeid(T));
	auto it = _components.find(id);
	if (generation == e.Generation && it != _components.end())
	{
		ComponentArray<T>* array = static_cast<ComponentArray<T>*>(_components[id].get());
		T* component = array->Get(e.ID);
		if (component != nullptr)
		{
			return *component;
		}
	}
	return T();
}

template<typename T>
inline bool World::HasComponent(Entity e)
{
	int generation = _entities[e.ID].Generation;

	std::type_index id = std::type_index(typeid(T));
	auto it = _components.find(id);
	if (generation == e.Generation && it != _components.end())
	{
		ComponentArray<T>* array = static_cast<ComponentArray<T>*>(_components[id].get());
		T* component = array->Get(e.ID);
		return component != nullptr;
	}
	return false;
}

template<typename T, typename ...Targs>
inline void World::TAttach(Entity e, T component, Targs ...components)
{
	int generation = _entities[e.ID].Generation;
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
	int generation = _entities[e.ID].Generation;
	if (generation == e.Generation)
	{
		std::bitset<MAX_COMPONENTS> componentMask = _componentMasks[e.ID];
		for (auto& it : _components)
		{
			IComponentArray* componentArray = it.second.get();
			componentArray->Remove(e.ID);
		}
		_entities[e.ID].Instantiated = false;
		_freeList.emplace(e.ID);
	}
}

inline void World::TAttach(Entity e)
{
	return;
}