#ifndef ELBA_TEST_HPP
#define ELBA_TEST_HPP

#include <elba.hpp>
#include <gtest/gtest.h>

class TestBase : public ::testing::Test
{
public:
	TestBase() : st(state) {}
	~TestBase() { EXPECT_EQ(0, st.size()); }

	elba::state state;
	elba::stack st;
};

#endif
