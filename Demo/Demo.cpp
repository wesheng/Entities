#include "Demo.h"

#include "World.h"
#include <iostream>


using namespace std;

struct Position
{
	int X = 0;

	Position(int x) {
		X = x;
	}
	Position() {

	}
};

struct Velocity
{
	int Vel = 0;

	Velocity(int vel) {
		Vel = vel;
	}
	Velocity() {

	}
};

void MovementSystem(World& world, Entity entity, Position position, Velocity velocity)
{
	position.X += velocity.Vel;

	world.Attach(entity, position);
}

int main()
{
	World w;
	Entity e1 = w.Create(Position{ 1 }, Velocity{ 6 });

	Position p = w.GetComponent<Position>(e1);
	Velocity v = w.GetComponent<Velocity>(e1);
	std::cout << p.X << ", " << v.Vel << std::endl;

	w.System<Position, Velocity>(MovementSystem);

	p = w.GetComponent<Position>(e1);
	v = w.GetComponent<Velocity>(e1);
	std::cout << p.X << ", " << v.Vel << std::endl;

	w.Remove(e1);

	w.System<Position, Velocity>(MovementSystem);
	p = w.GetComponent<Position>(e1);

	return 0;
}