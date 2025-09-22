# CMakeList.txt : CMake project for Entities, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library (Entities   "World.h" "World.cpp" "Entity.h" "Entity.cpp" "ComponentArray.h" "ComponentArray.cpp" "Query.h" "Query.cpp" "Utilities.h" "Utilities.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Entities PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
