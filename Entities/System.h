#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>
#include "ComponentArray.h"
#include <functional>
#include <vector>
#include "Query.h"

class System
{
public:
	// System();
	void Run();
private:

	//(void* callback)(World* world, std::vector<T*>... components);
};
