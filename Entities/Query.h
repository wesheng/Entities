#pragma once

#include <typeindex>
#include <unordered_map>
#include <memory>
#include "Entity.h"
#include "ComponentArray.h"

struct Query
{
};

template<typename T>
struct QueryIncludes : public Query
{
	std::type_info Info;

	QueryIncludes()
	{
		Info = std::type_info(typeid(T));
	}

	bool Evaluate(std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> components)
	{
		return false;
	}
};

template<typename T>
struct QueryExcludes : public Query
{
	std::type_info Info;

	QueryExcludes()
	{
		Info = std::type_info(typeid(T));
	}

	bool Evaluate(std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> components)
	{
		return false;
	}
};

/// <summary>
/// Anyway describes optional components.
/// </summary>
template<typename T>
struct QueryAnyway : public Query
{
	std::type_info Info;

	QueryAnyway()
	{
		Info = std::type_info(typeid(T));
	}

	bool Evaluate(std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> components)
	{
		return false;
	}
};

template<typename T1, typename T2>
struct QueryCombine : public Query
{
	T1 A;
	T2 B;

	QueryCombine(T1 a, T2 b)
	{
		A = a;
		B = b;
	}

	bool Evaluate(std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> components)
	{
		return false;
	}
};