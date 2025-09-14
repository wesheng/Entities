#pragma once

/// <summary>
/// An entity identifier.
/// </summary>
struct Entity
{
public:
	/// <summary>
	/// The identification assigned to the entity.
	/// </summary>
	size_t ID;
	/// <summary>
	/// The current generation of the entity, i.e. how many times this ID has been used by previously deleted entities.
	/// This also serves as an identifier to allow IDs to be recycled.
	/// </summary>
	int Generation;
};
