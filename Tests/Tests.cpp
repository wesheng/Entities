#include <gtest/gtest.h>
#include "World.h"

struct TestComponent
{
	int value = 1;
};

struct TestComponent2
{
	float value = 0.2f;
};

TEST(Entities, CreateWorld)
{
	World w;
	SUCCEED();
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
	
	TestComponent expected;
	expected.value = 4;
	w.Attach(e, expected);

	TestComponent actual = w.GetComponent<TestComponent>(e);
	EXPECT_EQ(actual.value, expected.value);
}

TEST(Entities, AddMultipleComponent)
{
	World w;
	Entity e = w.Create();

	TestComponent expected1;
	expected1.value = 3;
	w.Attach(e, expected1);

	TestComponent2 expected2;
	expected2.value = 5.2f;
	w.Attach(e, expected2);

	TestComponent actual1 = w.GetComponent<TestComponent>(e);
	EXPECT_EQ(actual1.value, expected1.value);

	TestComponent2 actual2 = w.GetComponent<TestComponent2>(e);
	EXPECT_EQ(actual2.value, expected2.value);
}

TEST(Entities, ReplaceSameComponent)
{
	World w;
	Entity e = w.Create();

	TestComponent expected1;
	expected1.value = 3;
	w.Attach(e, expected1);

	TestComponent expected2;
	expected2.value = 5;
	w.Attach(e, expected2);

	TestComponent actual = w.GetComponent<TestComponent>(e);
	EXPECT_EQ(actual.value, expected2.value);
}

TEST(Entities, HasComponent)
{
	World w;
	Entity e = w.Create();

	TestComponent expected;
	expected.value = 10;
	w.Attach(e, expected);

	EXPECT_TRUE(w.HasComponent<TestComponent>(e));
}

TEST(Entities, UpdateComponent)
{
	World w;
	Entity e = w.Create();
	TestComponent expected;
	expected.value = 2;
	w.Attach(e, expected);

	expected.value = 4;
	w.Attach(e, expected);

	TestComponent actual = w.GetComponent<TestComponent>(e);
	EXPECT_EQ(actual.value, expected.value);
}

TEST(Entities, DetachComponent)
{
	World w;
	Entity e = w.Create();
	TestComponent expected;
	expected.value = 3;
	w.Attach(e, expected);

	w.Detach<TestComponent>(e);

	TestComponent actual = w.GetComponent<TestComponent>(e);
	EXPECT_NE(actual.value, expected.value);
}

void TestSystem(World& world, Query<TestComponent> query) {
	TestComponent c = query.Get<TestComponent>();
	c.value += 4;
	world.Attach(query.Entity, c);
}

TEST(Entities, RunSystem)
{
	World w;
	Entity e = w.Create();
	TestComponent expected;
	expected.value = 3;
	w.Attach(e, expected);

	w.System(TestSystem);

	expected.value += 4;
	TestComponent actual = w.GetComponent<TestComponent>(e);

	EXPECT_EQ(actual.value, expected.value);
}