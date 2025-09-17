#include <gtest/gtest.h>
#include "World.h"

struct TestComponent
{
	int value = 1;
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