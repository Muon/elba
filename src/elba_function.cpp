#include "elba_function.hpp"

#include <stdexcept>

extern "C"
{
#include <lua.h>
}

namespace elba
{

function::function() {}

function::function(lua_State* L)
	: reference(L)
{
}

function::function(lua_State* L, int index)
	: reference(L)
{
	stack st(L);

	if(st.element_type(index) != stack::function)
	{
		throw std::runtime_error("stack index given not a table");
	}


	lua_pushvalue(L, index);
	set_ref();
}

value_ref function::operator()()
{
	push_ref();

	stack st(L);

	call(0, 1);

	value_ref ref(L, stack::top);

	st.pop(1);

	return ref;
}

void function::call(int nargs, int nresults)
{
	lua_call(L, nargs, nresults);
}

}
