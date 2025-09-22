#pragma once

#include <typeindex>
#include <unordered_map>
#include <memory>
#include "Entity.h"
#include "ComponentArray.h"
#include "Utilities.h"
#include <bitset>


/// <summary>
/// Designates components that will be included in a system.
/// </summary>
/// <typeparam name="...T">The components to include.</typeparam>
template<typename ...T>
struct Query
{
	/// <summary>
	/// The Entity.
	/// </summary>
	Entity Entity;
	/// <summary>
	/// The components retrieved.
	/// </summary>
	/// <remarks>To retrieve, you can use the Get() function.</remarks>
	std::tuple<T...> Components;

	/// <summary>
	/// Retrieves a component.
	/// </summary>
	/// <typeparam name="U">The component type.</typeparam>
	/// <returns>The component.</returns>
	template<typename U>
	U Get() {
		return std::get<U>(Components);
	}
};

/// <summary>
/// Designates components that will be excluded in a system.
/// </summary>
/// <typeparam name="...U"></typeparam>
template<typename ...U>
struct Without
{
};