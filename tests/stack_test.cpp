#include "elba_test.hpp"

class StackTest : public TestBase
{
};

const char* f() { return "foo"; }

TEST_F(StackTest, PushNullaryFreeFunction)
{
	st.push(f);
	st.call(0, 1);
	EXPECT_STREQ("foo", st.get<const char*>(-1));
	st.pop(1);
}

const char* g(const char* s) { EXPECT_STREQ("foo", s); return s; }

TEST_F(StackTest, PushUnaryFreeFunction)
{
	st.push(g);
	st.push("foo");
	st.call(1, 1);
	EXPECT_STREQ("foo", st.get<const char*>(-1));
	st.pop(1);
}
