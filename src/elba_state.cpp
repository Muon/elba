#include "../include/elba_state.hpp"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <stdexcept>

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
			size_t len;
			const char* error = lua_tolstring(L, -1, &len);
			if(len > 0)
			{
				throw std::runtime_error(error);
			}
			
			if(ret == LUA_ERRRUN)
			{
				throw std::runtime_error("runtime error while executing string");
			}
			else if(ret == LUA_ERRMEM)
			{
				throw std::runtime_error("ran out of memory while executing string");
			}
			else if(ret == LUA_ERRERR)
			{
				throw std::runtime_error("error occurred running the error handler while executing string");
			}
			else
			{
				throw std::runtime_error("unknown error occurred while executing string");
			}
		}
	}
	else
	{
		size_t len;
		const char* error = lua_tolstring(L, -1, &len);
		if(len > 0)
		{
			throw std::runtime_error(error);
		}
		
		if(ret == LUA_ERRSYNTAX)
		{
			throw std::runtime_error("syntax error in string");
		}
		else if(ret == LUA_ERRMEM)
		{
			throw std::runtime_error("ran out of memory while parsing string");
		}
		else
		{
			throw std::runtime_error("unknown error occurred while parsing string");
		}
	}
}

}
