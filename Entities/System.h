#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>
#include "ComponentArray.h"
#include <functional>
#include <vector>

//template<typename ... T>
//class System
//{
//public:
//	System(std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> createdComponents, std::function<void(World* world, std::vector<T*>... components)> callback);
//	void Run();
//private:
//	std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> _components;
//	std::function<void(World* world, std::vector<T*>... components)> _callback;
//};
//
//template<typename ...T>
//inline void System<T...>::Run()
//{
//
//}
