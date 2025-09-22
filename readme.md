# Entities
A simple implementation of the Entity-Component-System (ECS), written in C++.

The project uses Cmake to enable flexibility in compiler choice. 

## Features
- Uses generational indexing to create new entities
- Stores components in a columnar fashion
- Uses bitmasks for component lookup – Up to 100 component types supported

## Limitations
- Not intuitive for behaviors dependent on another entity
- Cannot create systems with optional components
- Makes use of the std library, which may have an impact on performance compared to direct implementations