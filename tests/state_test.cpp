#include "elba_test.hpp"

class StateTest : public TestBase
{
};

TEST_F(StateTest, CastToLuaStatePointer)
{
	lua_State* L = state;
	ASSERT_TRUE(L != NULL);
}

TEST_F(StateTest, RunEmptyString)
{
	state.do_string("");
}

TEST_F(StateTest, GetGlobalsTable)
{
	elba::reference globals = state.globals();
	ASSERT_EQ(elba::stack::table, globals.type());
}
