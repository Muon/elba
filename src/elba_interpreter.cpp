#include "../include/elba_interpreter.hpp"

extern "C"
{
#include <lua.hpp>
#include <lauxlib.h>
}

namespace elba
{

interpreter::interpreter()
	: state(luaL_newstate())
{
	if(!state)
	{
		throw;
	}
}

interpreter::~interpreter()
{
	lua_close(state);
}

void interpreter::openLibs()
{
	luaL_openlibs(state);
}

void interpreter::doString(const char* str)
{
	int ret = luaL_loadstring(state, str);

	if(ret == 0)
	{
		ret = lua_pcall(state, 0, LUA_MULTRET, 0);

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