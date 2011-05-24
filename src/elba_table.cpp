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
	: reference(L)
	, L(L)
	, cur_stack(L)
{
	lua_newtable(L);
}

table::table(lua_State* L, int index)
	: reference(L, index)
	, L(L)
	, cur_stack(L)
{
	if(cur_stack.element_type(index) != stack::table)
	{
		throw std::runtime_error("stack index given not a table");
	}
}

table::table(lua_State* L, int num_array, int num_assoc)
	: reference(L)
	, L(L)
	, cur_stack(L)
{
	lua_createtable(L, num_array, num_assoc);
}

void table::get_table_field() const
{
	lua_gettable(L, get_index());
}

void table::set_table_field() const
{
	lua_settable(L, get_index());
}

}
