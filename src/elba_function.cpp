#include "elba_function.hpp"

#include <stdexcept>

extern "C"
{
#include <lua.h>
}

namespace elba
{

function::function(lua_State* L)
	: reference(L)
	, L(L)
{
}

function::function(lua_State* L, int index)
	: reference(L)
	, L(L)
{
	stack st(L);

	if(st.element_type(index) != stack::function)
	{
		throw std::runtime_error("stack index given not a table");
	}


	lua_pushvalue(L, index);
	set_ref();
}

void function::call(int nargs, int nresults)
{
	lua_call(L, nargs, nresults);
}

}
