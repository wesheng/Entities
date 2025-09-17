# Entities
A simple implementation of the Entity-Component-System (ECS), written in C++. It was written for the author to gain familarity with the architecture behind such a system.

The project uses Cmake to enable flexibility in compiler choice.

Note that this project is incomplete and may not be stable – do not use this in a production setting.

## Features
- Uses generational indexing to create new entities
- Stores components in a columnar fashion
- Uses bitmasks for component lookup – Up to 100 component types supported