// ECS.cpp : Defines the entry point for the application.
//

#include "Entities.h"

#include "World.h"

using namespace std;

struct Position
{
	int X;

	Position(int x) {
		X = x;
	}
};

struct Velocity
{
	int Vel;

	Velocity(int vel) {
		Vel = vel;
	}
};

int main()
{
	World w;
	Entity e1 = w.Create(Position{1}, Velocity{6});
	
	Position* p = w.GetComponent<Position>(e1);
	Velocity* v = w.GetComponent<Velocity>(e1);
	std::cout << p->X << ", " << v->Vel << std::endl;

	std::function<void(World* world, Entity e, Position& position, Velocity& velocity)> movementSystem = [](World* world, Entity entity, Position& position, Velocity& velocity) {
			position.X += velocity.Vel;
		};

	w.System<Position, Velocity>(movementSystem);

	p = w.GetComponent<Position>(e1);
	v = w.GetComponent<Velocity>(e1);
	std::cout << p->X << ", " << v->Vel << std::endl;

	return 0;
}
