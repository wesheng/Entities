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

void CheckPosition(World& world, Entity entity, Position position)
{
	std::cout << "Entity " << entity.ID << " position: " << position.X << std::endl;
}

void Move(World& world, Query<Position, Velocity> query)
{
	Position pos = query.Get<Position>();
	Velocity vel = query.Get<Velocity>();
	pos.X += vel.Vel;
	world.Attach(query.Entity, pos);
}

void StaticMovement(World& world, Query<Position> query, Without<Velocity> without)
{
	Position pos = query.Get<Position>();
	std::cout << "Stationary: " << pos.X << std::endl;
}

int main()
{
	World w;
	Entity e1 = w.Create(Position{ 1 }, Velocity{ 6 });
	Entity e2 = w.Create(Position{ 7 });

	w.SystemQ(Move);
	w.SystemQ(StaticMovement);
	//w.System<Position>(CheckPosition);
	//std::cout << "---" << std::endl;

	//w.System<Position, Velocity>(MovementSystem);
	//w.System<Position>(CheckPosition);
	//std::cout << "---" << std::endl;

	w.Remove(e1);

	//w.System<Position, Velocity>(MovementSystem);
	//w.System<Position>(CheckPosition);

	return 0;
}