#include <elba_reference.hpp>

extern "C"
{
#include <lua.h>
}

#include <cassert>
#include <stdexcept>

namespace elba
{

reference::reference(lua_State* L)
	: cur_stack(L)
	, stack_index(cur_stack.size() + 1)
{
}

reference::reference(lua_State* L, int index)
	: cur_stack(L)
{
	if(index <= LUA_REGISTRYINDEX) // if it's a special index, set it
	{
		stack_index = index;
	}
	else if(index < 0) // if it's a relative index, calculate the real one
	{
		stack_index = cur_stack.size() + index + 1;
	}
	else
	{
		stack_index = index;
	}
	
	// make sure that it exists
	if(!cur_stack.is_valid_index(stack_index))
	{
		throw std::runtime_error("invalid stack index");
	}
}

reference::~reference()
{
	// ignore special indices
	if(stack_index > 0)
	{
		assert(cur_stack.size() == stack_index);
		cur_stack.pop(1);
	}
}

}
