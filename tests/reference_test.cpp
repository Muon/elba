#include "elba_test.hpp"

class ReferenceTest : public TestBase
{
};

// REF_EQ() is for testing operator==() reflexivity, which I have had problems with before
#define REF_EQ_(expected, value) (((expected) == (value)) && ((value) == (expected)))
#define REF_NE_(expected, value) (((expected) != (value)) && ((value) != (expected)))
#define REF_EQ(expected, value) EXPECT_TRUE(REF_EQ_(expected, value) && !REF_NE_(expected, value))
#define REF_NE(expected, value) EXPECT_TRUE(REF_NE_(expected, value) && !REF_EQ_(expected, value))

class SingleRefTest : public ReferenceTest
{
public:
	SingleRefTest() : ref(elba::reference(state)) {}

	elba::reference ref;
};

TEST_F(SingleRefTest, EmptyRefIsNil)
{
	EXPECT_EQ(elba::stack::nil, ref.type()) << "where ref is of type " << st.type_name(ref.type());
	REF_EQ(elba::nil, ref);
}

TEST_F(ReferenceTest, RefIsConstructibleFromStack)
{
	st.push("foo");
	elba::reference ref(state, -1);
	REF_EQ("foo", ref);
	st.pop(1);
}

TEST_F(ReferenceTest, EqualRefRefComparison)
{
	elba::reference ref1(state);
	ref1 = "foo";
	elba::reference ref2(state);
	ref2 = "foo";
	REF_EQ(ref1, ref2);
}

TEST_F(ReferenceTest, UnequalRefRefComparison)
{
	elba::reference ref1(state);
	ref1 = "foo";
	elba::reference ref2(state);
	ref2 = "bar";
	REF_NE(ref1, ref2);
}

TEST_F(SingleRefTest, EqualRefValComparison)
{
	ref = "foo";
	REF_EQ("foo", ref);
}

TEST_F(SingleRefTest, UnequalRefValComparison)
{
	ref = "foo";
	REF_NE("bar", ref);
}

TEST_F(SingleRefTest, PrimitiveAssignment)
{
	ref = "foo";
	EXPECT_EQ("foo", ref);
}

TEST_F(ReferenceTest, RefCopying)
{
	elba::reference ref1(state);
	ref1 = "foo";

	elba::reference ref2(state);
	ref2 = ref1;

	EXPECT_EQ(ref1, ref2);
}

TEST_F(SingleRefTest, RefGetting)
{
	ref = "foo";
	EXPECT_EQ("foo", ref.get<std::string>());
}

TEST_F(SingleRefTest, RefCasting)
{
	ref = "foo";
	std::string value = ref;
	EXPECT_EQ("foo", value);
}

class RefTableTest : public ReferenceTest
{
public:
	RefTableTest() : table(elba::make_table(state))
	{
		table.set("foo", "bar");
	}
	elba::reference table;
};

TEST_F(RefTableTest, MakeTable)
{
	EXPECT_EQ(elba::stack::table, table.type());
}

TEST_F(RefTableTest, TableSetGet)
{
	EXPECT_EQ("bar", table.get<elba::reference>("foo"));
}

TEST_F(RefTableTest, EqualIndexIndexComparison)
{
	REF_EQ(table["foo"], table["foo"]);
}

TEST_F(RefTableTest, UnequalIndexIndexComparison)
{
	table["quux"] = "xyzzy";
	REF_NE(table["quux"], table["foo"]);
}

TEST_F(RefTableTest, EqualIndexValComparison)
{
	REF_EQ("bar", table["foo"]);
}

TEST_F(RefTableTest, UnequalIndexValComparison)
{
	REF_NE("quux", table["foo"]);
}

TEST_F(RefTableTest, EqualIndexRefComparison)
{
	elba::reference ref(state);
	ref = "bar";
	REF_EQ(ref, table["foo"]);
}

TEST_F(RefTableTest, UnequalIndexRefComparison)
{
	elba::reference ref(state);
	ref = "quux";
	REF_NE(ref, table["foo"]);
}

TEST_F(RefTableTest, TableIndexSetGet)
{
	EXPECT_EQ("bar", table["foo"]);
}

TEST_F(RefTableTest, TableIndexSubindexSetGet)
{
	{
		elba::reference child = elba::make_table(state);
		child["bar"] = "quux";
		table["foo"] = child;
	}
	EXPECT_EQ("quux", table["foo"]["bar"]);
}

TEST_F(RefTableTest, EmptyRefMetatableGet)
{
	EXPECT_EQ(elba::nil, table.metatable());
}

class RefMetatableTest : public RefTableTest
{
public:
	RefMetatableTest() : metatable(elba::make_table(state))
	{
		table.metatable(metatable);

		elba::reference index = elba::make_table(state);
		index.set("xyzzy", "bar");
		metatable.set("__index", index);
	}

	elba::reference metatable;
};

TEST_F(RefMetatableTest, RefMetatableSetGet)
{
	EXPECT_EQ(metatable, table.metatable());
}

TEST_F(RefMetatableTest, RefRawGet)
{
	EXPECT_EQ("bar", table.get<std::string>("xyzzy"));
	EXPECT_EQ(elba::nil, table.raw_get<elba::reference>("xyzzy"));
}

TEST_F(RefMetatableTest, RefRawSet)
{
	table.raw_set("foo", "quux");
	EXPECT_EQ("quux", table.get<std::string>("foo"));
}
