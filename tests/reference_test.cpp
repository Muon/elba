#include "elba_test.hpp"

class ReferenceTest : public TestBase
{
};

// REF_EQ() is for testing operator==() reflexivity, which I have had problems with before
#define REF_EQ(expected, value) EXPECT_TRUE(((expected) == (value)) && ((value) == (expected)))
#define REF_NE(expected, value) EXPECT_TRUE(((expected) != (value)) && ((value) != (expected)))

TEST_F(ReferenceTest, EmptyRefIsNil)
{
	elba::reference ref(state);

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
	st.push("foo");
	elba::reference ref1(state, -1);
	elba::reference ref2(state, -1);
	REF_EQ(ref1, ref2);
	st.pop(1);
}

TEST_F(ReferenceTest, UnequalRefRefComparison)
{
	st.push("foo");
	elba::reference ref1(state, -1);
	st.push("bar");
	elba::reference ref2(state, -1);
	REF_NE(ref1, ref2);
	st.pop(2);
}


TEST_F(ReferenceTest, EqualRefValComparison)
{
	st.push("foo");
	elba::reference ref(state, -1);
	REF_EQ("foo", ref);
	st.pop(1);
}

TEST_F(ReferenceTest, UnequalRefValComparison)
{
	st.push("foo");
	elba::reference ref(state, -1);
	REF_NE("bar", ref);
	st.pop(1);
}

TEST_F(ReferenceTest, PrimitiveAssignment)
{
	elba::reference ref(state);
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

TEST_F(ReferenceTest, MakeTable)
{
	elba::reference table = elba::make_table(state);
	EXPECT_EQ(elba::stack::table, table.type());
}

TEST_F(ReferenceTest, TableSetGet)
{
	elba::reference table = elba::make_table(state);
	table.set("foo", "bar");
	EXPECT_EQ("bar", table.get<elba::reference>("foo"));
}

TEST_F(ReferenceTest, EqualIndexIndexComparison)
{
	elba::reference table = elba::make_table(state);
	table["foo"] = "bar";
	REF_EQ(table["foo"], table["foo"]);
}

TEST_F(ReferenceTest, UnequalIndexIndexComparison)
{
	elba::reference table = elba::make_table(state);
	table["foo"] = "bar";
	table["quux"] = "xyzzy";
	REF_NE(table["quux"], table["foo"]);
}

TEST_F(ReferenceTest, EqualIndexValComparison)
{
	elba::reference table = elba::make_table(state);
	table["foo"] = "bar";
	REF_EQ("bar", table["foo"]);
}

TEST_F(ReferenceTest, UnequalIndexValComparison)
{
	elba::reference table = elba::make_table(state);
	table["foo"] = "bar";
	REF_NE("quux", table["foo"]);
}

TEST_F(ReferenceTest, EqualIndexRefComparison)
{
	elba::reference table = elba::make_table(state);
	table["foo"] = "bar";
	elba::reference ref(state);
	ref = "bar";
	REF_EQ(ref, table["foo"]);
}

TEST_F(ReferenceTest, UnequalIndexRefComparison)
{
	elba::reference table = elba::make_table(state);
	table["foo"] = "bar";
	elba::reference ref(state);
	ref = "quux";
	REF_NE(ref, table["foo"]);
}

TEST_F(ReferenceTest, TableIndexSetGet)
{
	elba::reference table = elba::make_table(state);
	table["foo"] = "bar";
	EXPECT_EQ("bar", table["foo"]);
}