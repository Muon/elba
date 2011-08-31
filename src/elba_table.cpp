#include "../include/elba_table.hpp"
#include "../include/elba_stack.hpp"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
}

#include <stdexcept>
#include <cstring>

namespace elba
{

table::table()
{
}

table::table(lua_State* L)
	: reference(L)
{
	lua_newtable(L);
	set_ref();
}

table::table(lua_State* L, int index)
	: reference(L)
{
	stack st(L);

	if(st.element_type(index) != stack::table)
	{
		throw std::runtime_error("stack index given not a table");
	}

	lua_pushvalue(L, index);
	set_ref();
}

table::table(lua_State* L, int num_array, int num_assoc)
	: reference(L)
{
	lua_createtable(L, num_array, num_assoc);
	set_ref();
}

std::ostream& operator<<(std::ostream& stream, const object_index& idx)
{
	std::string str = idx;
	stream << str;
	return stream;
}

}
