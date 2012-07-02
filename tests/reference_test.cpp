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

