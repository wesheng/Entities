# Entities
A simple implementation of the Entity-Component-System (ECS), written in C++. This project was created to gain familarity of ECS architecture, and to gain an understanding of STL structures provided by modern C++.

Implementation is partly inspired by the work conducted by Redmond, et. al (2025). Note that this isn't an exact implementation. 

The project uses Cmake to enable flexibility in compiler choice. 

## Features
- Uses generational indexing to create new entities
- Stores components in a columnar fashion
- Uses bitmasks for component lookup – Up to 100 component types supported

## Limitations
- Not intuitive for behaviors dependent on another entity
- Cannot create systems with optional components

## References

```
Redmond, P., Castello, J., Trilla, J. M. C., & Kuper, L. (2025). Exploring the Theory and Practice of Concurrency in the Entity-Component-System Pattern (arXiv:2508.15264). arXiv. https://doi.org/10.48550/arXiv.2508.15264
```
