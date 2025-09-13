#pragma once

#include "Entity.h"
#include "Component.h"
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

const unsigned int MAX_COMPONENTS = 100;
static unsigned int Component_Count = 0;

template<typename T>
unsigned int GetComponentId() {
	static unsigned int id = Component_Count++;
	return id;
}

class World
{
public:
	template<typename... T>
	Entity Create(T... components);
	template<typename... T>
	void Attach(Entity e, T... components);
	template<class T>
	void Detach(Entity e);

	//template<typename... T>
	//void System(std::function<void(World* world, std::vector<std::tuple<T*...>> components)> callback);
	template<typename... T>
	void System(std::function<void(World* world, Entity entity, T&... components)> callback);

	template<class T>
	T* GetComponent(Entity e);
private:
	template<typename ... Targs>
	std::tuple<Targs* ...> GetComponents(Entity e);

	template<typename ... Targs>
	std::bitset<MAX_COMPONENTS> GetMask();

	template<typename T, typename... Targs>
	void TAttach(Entity e, T component, Targs... components);
	// Basecase of above function. Does nothing.
	void TAttach(Entity e);

private:
	std::vector<int> _entities;
	std::queue<int> _freeList;
	std::vector<std::bitset<MAX_COMPONENTS>> _componentMasks;

	std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> _components;
};

template<typename... T>
inline Entity World::Create(T... components)
{
	size_t index = _entities.size();
	int generation = 0;
	if (_freeList.empty())
	{
		index = _entities.size();
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

template<class T>
inline void World::Detach(Entity e)
{
	std::type_index id = std::type_index(typeid(T));
	auto it = _components.find(id);
	if (it != _components.end())
	{
		ComponentArray<T>* array = static_cast<ComponentArray<T>*>(_components[id].get());
		return array->Remove(e.ID);
	}
}

//template<typename ...T>
//inline System<T...> World::MakeSystem(std::function<void(World* world, std::vector<T*...>components)> callback)
//{
//
//	return System();
//}

//template<typename ...T>
//inline void World::System(std::function<void(World* world, std::vector<std::tuple<T*...>> components)> callback)
//{
//	std::bitset<MAX_COMPONENTS> mask = GetMask<T...>();
//
//	std::vector<std::tuple<T*...>> entityComponentList;
//	for (uint32_t entityId = 0; entityId < _componentMasks.size(); entityId++)
//	{
//		if ((_componentMasks[entityId] & mask) == mask) {
//			Entity e;
//			e.ID = entityId;
//			e.Generation = _entities[entityId];
//
//			std::tuple<T*...> entityComponents = std::make_tuple<T*...>(GetComponent<T>(e)...);
//
//			entityComponentList.push_back(entityComponents);
//		}
//	}
//
//	callback(this, entityComponentList);
//}

template<typename ...T>
inline void World::System(std::function<void(World* world, Entity e, T&...components)> callback)
{
	std::bitset<MAX_COMPONENTS> mask = GetMask<T...>();

	for (uint32_t entityId = 0; entityId < _componentMasks.size(); entityId++)
	{
		if ((_componentMasks[entityId] & mask) == mask) {
			Entity e;
			e.ID = entityId;
			e.Generation = _entities[entityId];

			// std::tuple<T*...> entityComponents = std::make_tuple<T*...>(GetComponent<T>(e)...);

			callback(this, e, (*GetComponent<T>(e))...);
		}
	}
}

template<class T>
inline T* World::GetComponent(Entity e)
{
	std::type_index id = std::type_index(typeid(T));
	auto it = _components.find(id);
	if (it != _components.end())
	{
		ComponentArray<T>* array = static_cast<ComponentArray<T>*>(_components[id].get());
		return array->Get(e.ID);
	}
	return nullptr;
}

template<typename ...T>
inline void World::TSystem(std::function<void(World* world, std::vector<std::tuple<T*...>> components)> callback)
{
	std::bitset<MAX_COMPONENTS> mask = TGetMask<T...>();

	std::vector<std::tuple<T*...>> entityComponentList;
	for (uint32_t entityId = 0; entityId < _componentMasks.size(); entityId++)
	{
		if ((_componentMasks[entityId] & mask) == mask) {
			Entity e;
			e.ID = entityId;
			e.Generation = _entities[entityId];

			std::tuple<T*...> entityComponents = std::make_tuple<T*...>(GetComponent<T*>(e)...);

			entityComponentList.push_back(entityComponents);
		}
	}

	callback(this, entityComponentList);
}

template<typename ...Targs>
inline std::tuple<Targs*...> World::GetComponents(Entity e)
{

	return std::tuple<Targs*...>();
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
	std::type_index id = std::type_index(typeid(T));
	auto it = _components.find(id);
	if (it == _components.end())
	{
		_components[id] = std::make_unique<ComponentArray<T>>();
	}

	ComponentArray<T>* array = static_cast<ComponentArray<T>*>(_components[id].get());
	array->Insert(e.ID, component);

	unsigned int componentId = GetComponentId<T>();
	_componentMasks[e.ID].set(componentId);

	TAttach(e, components...);
}

inline void World::TAttach(Entity e)
{
	return;
}