#pragma once

#include <typeindex>
#include <unordered_map>
#include <memory>
#include "Entity.h"
#include "ComponentArray.h"
#include "Utilities.h"
#include <bitset>


template<typename ...T>
struct Query
{
	Entity Entity;
	std::tuple<T...> Components;

	template<typename U>
	U Get() {
		return std::get<U>(Components);
	}
};

template<typename ...U>
struct Without
{
};


//struct Query
//{
//};
//
///// <summary>
///// Indicates which Components to use.
///// </summary>
///// <typeparam name="...T"></typeparam>
//template<typename ... T>
//struct QIncludes : public Query
//{
//};
//
//template<typename ... T>
//struct QExcludes : public Query
//{
//	bool Evaluate(std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> components)
//	{
//		return false;
//	}
//};
//
///// <summary>
///// Anyway describes optional components.
///// </summary>
//template<typename T>
//struct QAnyway : public Query
//{
//	std::type_info Info;
//
//	QAnyway()
//	{
//		Info = std::type_info(typeid(T));
//	}
//
//	bool Evaluate(std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> components)
//	{
//		return false;
//	}
//};
//
//template<typename T1, typename T2>
//struct QueryCombine : public Query
//{
//	T1 A;
//	T2 B;
//
//	bool Evaluate(std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> components)
//	{
//		return false;
//	}
//};
//
//struct Q
//{
//public:
//	template<typename ...T>
//	Q& Include()
//	{
//		std::bitset<MAX_COMPONENTS> mask = GetMask<T...>();
//		_excludeMask = !mask & _excludeMask;
//		_includeMask |= mask;
//		return *this;
//	}
//
//	template<typename ...T>
//	Q& Exclude()
//	{
//		std::bitset<MAX_COMPONENTS> mask = GetMask<T...>();
//		_includeMask = !mask & _includeMask;
//		_excludeMask |= mask;
//		return *this;
//	}
//
//	template<typename ...T>
//	Q& Anyway()
//	{
//		_anywayMask != GetMask<T...>();
//		return *this;
//	}
//
//	Q& Join(Q query)
//	{
//		_includeMask |= query._includeMask;
//		_excludeMask |= query._excludeMask;
//		_anywayMask |= query._anywayMask;
//		return *this;
//	}
//private:
//	std::bitset<MAX_COMPONENTS> _includeMask;
//	std::bitset<MAX_COMPONENTS> _excludeMask;
//	std::bitset<MAX_COMPONENTS> _anywayMask;
//};