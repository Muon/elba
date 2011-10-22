#include <elba_state.hpp>
#include <elba_stack.hpp>
#include <elba_error.hpp>

extern "C"
{
#include <lauxlib.h>
#include <lualib.h>
}

namespace elba
{

state::state_destruction_deferral::state_destruction_deferral()
	: L(luaL_newstate())
{
	if(!L)
		throw elba::out_of_memory_error(L, "ran out of memory while creating Lua state");
}

state::state_destruction_deferral::~state_destruction_deferral()
{
	lua_close(L);
}

state::state() : globals(state_container.L, LUA_GLOBALSINDEX)
{
}

state::operator lua_State* () const
{
	return state_container.L;
}

void state::open_libs() const
{
	luaL_openlibs(state_container.L);
}

void state::do_string(const std::string& string) const
{
	stack st(*this);
	st.create_function(string);
	st.call(0, 0);
}

reference state::create_table() const
{
	return make_table(*this);
}

}
