#include <elba.hpp>
#include <gtest/gtest.h>

class TestBase : public ::testing::Test
{
public:
	TestBase() : st(state) {}
	virtual void TearDown() { ASSERT_EQ(0, st.size()); }

	elba::state state;
	elba::stack st;
};
