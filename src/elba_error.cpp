#include <elba_error.hpp>

extern "C"
{
#include <lua.h>
}

namespace elba
{

runtime_error::runtime_error(lua_State* L, const std::string& msg)
	: std::runtime_error(msg)
	, error(L)
{
	lua_Debug dbg;
	for(int i = 1; lua_getstack(L, i, &dbg) == 1; ++i)
	{
		call_stack_.push_back(activation_record(L, dbg));
	}
}

}
