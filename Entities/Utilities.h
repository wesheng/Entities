#pragma once

#include <bitset>

/// <summary>
/// THe max number of components.
/// </summary>
const unsigned int MAX_COMPONENTS = 100;

static unsigned int ComponentCount = 0;

/// <summary>
/// Assigns and retrieves a component identifier.
/// </summary>
/// <typeparam name="T">The component to obtain an identifier from.</typeparam>
/// <returns>The identifier.</returns>
template<typename T>
unsigned int GetComponentId() {
	static unsigned int id = ComponentCount++;
	return id;
}

/// <summary>
/// Retrieves the bitmask from specified components.
/// </summary>
/// <typeparam name="...Targs"></typeparam>
/// <returns></returns>
template<typename ...Targs>
std::bitset<MAX_COMPONENTS> GetMask()
{
	std::bitset<MAX_COMPONENTS> mask;

	(mask.set(GetComponentId<Targs>()), ...);

	return mask;
}