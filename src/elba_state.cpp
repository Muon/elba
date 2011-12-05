#include <elba_state.hpp>
#include <elba_stack.hpp>
#include <elba_error.hpp>

extern "C"
{
#include <lauxlib.h>
#include <lualib.h>
}

#include <iostream>

namespace elba
{

state::state() : L(luaL_newstate())
{
	if(!L)
		throw elba::out_of_memory_error(L, "ran out of memory while creating Lua state");
}

state::~state()
{
	std::cout << "state destroyed" << std::endl;
	lua_close(L);
}

state::operator lua_State* () const
{
	return L;
}

void state::open_libs() const
{
	luaL_openlibs(L);
}

void state::do_string(const std::string& string) const
{
	stack st(*this);
	st.create_function(string);
	st.call(0, 0);
}

reference state::globals() const
{
	return reference(L, stack::globals_index());
}

reference state::create_table() const
{
	return make_table(*this);
}

}
