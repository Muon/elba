#include "../include/elba_stack.hpp"


extern "C"
{
#include <lua.h>
}

namespace elba
{

stack::stack(lua_State* L)
	: L(L)
{
}

void stack::push(const std::string& string) const
{
	lua_pushstring(L, string.c_str());
}

void stack::push(const char* string) const
{
	lua_checkstack(L, 1);
	lua_pushstring(L, string);
}

void stack::get(std::string& string, int index) const
{
	string = lua_tostring(L, index);
}

void stack::push(long int integer) const
{
	lua_pushinteger(L, integer);
}

void stack::get(long int& integer, int index) const
{
	integer = lua_tointeger(L, index);
}

void stack::pop(int num) const
{
	lua_pop(L, num);
}

int stack::size() const
{
	return lua_gettop(L);
}

bool stack::is_valid_index(int index) const
{
	return !lua_isnone(L, index);
}

stack::type stack::element_type(int index) const
{
	return static_cast<stack::type>(lua_type(L, index));
}

}
