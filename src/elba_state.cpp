#include <elba_state.hpp>
#include <elba_stack.hpp>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <stdexcept>

namespace elba
{

static int panic(lua_State* L)
{
	stack st(L);

	std::string error_message = st.get<std::string>(stack::top);
	st.pop(1);

	throw std::runtime_error(error_message);
}

state::state_destruction_deferral::state_destruction_deferral()
	: L(luaL_newstate())
{
	if(!L)
		throw std::runtime_error("ran out of memory while creating Lua state");

	lua_atpanic(L, panic);
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
