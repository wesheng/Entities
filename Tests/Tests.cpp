#include <gtest/gtest.h>
#include "World.h"

struct TestComponent
{
	int value = 1;
};

TEST(Entities, CreateWorld)
{
	World w;
}

TEST(Entities, CreateEntity)
{
	World w;
	Entity e1 = w.Create();
	EXPECT_GE(e1.ID, 0);
	EXPECT_GE(e1.Generation, 1);

	Entity e2 = w.Create();
	EXPECT_NE(e1, e2);
}

TEST(Entities, DeleteAndCreateEntity)
{
	World w;
	Entity e1 = w.Create();
	w.Remove(e1);

	Entity e2 = w.Create();
	EXPECT_EQ(e1.ID, e2.ID);
	EXPECT_NE(e1, e2);
}

TEST(Entities, AddComponent)
{
	World w;
	Entity e = w.Create();
	
	TestComponent component;
	component.value = 4;
	w.Attach(e, component);

	TestComponent tc = w.GetComponent<TestComponent>(e);
	EXPECT_EQ(tc.value, component.value);
}