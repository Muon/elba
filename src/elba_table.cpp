#include "../include/elba_table.hpp"
#include "../include/elba_stack.hpp"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
}

#include <stdexcept>

namespace elba
{

table::table(lua_State* L)
	: L(L)
	, cur_stack(L)
{
	lua_newtable(L);
	stack_index = cur_stack.size();
}

table::table(lua_State* L, int index)
	: L(L)
	, cur_stack(L)
{
	if(index <= LUA_REGISTRYINDEX)
	{
		stack_index = index;
		return;
	}
	
	if(index < 0)
	{
		stack_index = cur_stack.size() + index + 1;
	}
	else
	{
		if(cur_stack.is_valid_index(index))
		{
			stack_index = index;
		}
		else
		{
			throw std::runtime_error("invalid stack index");
		}
	}
	
	if(cur_stack.element_type(index) != stack::table)
	{
		throw std::runtime_error("stack index given not a table");
	}
}

table::table(lua_State* L, int num_array, int num_assoc)
	: L(L)
	, cur_stack(L)
{
	lua_createtable(L, num_array, num_assoc);
}

void table::get_table_field() const
{
	lua_gettable(L, stack_index);
}

void table::set_table_field() const
{
	lua_settable(L, stack_index);
}

}
