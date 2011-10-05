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

void state::do_string(const char* string) const
{
	int ret = luaL_loadstring(state_container.L, string);

	if(ret == 0)
	{
		ret = lua_pcall(state_container.L, 0, LUA_MULTRET, 0);

		if(ret == 0)
		{
			return;
		}
		else
		{
			std::size_t len;
			const char* error = lua_tolstring(state_container.L, -1, &len);
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
		std::size_t len;
		const char* error = lua_tolstring(state_container.L, -1, &len);
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

reference state::create_table() const
{
	return make_table(*this);
}

}
