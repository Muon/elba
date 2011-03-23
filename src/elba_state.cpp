#include "../include/elba_state.hpp"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace elba
{

state::state()
	: L(luaL_newstate())
	, globals(L, LUA_GLOBALSINDEX)
{
	if(!L)
		throw;
}

state::~state()
{
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

void state::do_string(const char* string) const
{
	int ret = luaL_loadstring(L, string);

	if(ret == 0)
	{
		ret = lua_pcall(L, 0, LUA_MULTRET, 0);

		if(ret == 0)
		{
			return;
		}
		else
		{
			throw;
		}
	}
	else
	{
		throw;
	}
}

}
