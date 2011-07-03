#include "../include/elba_stack.hpp"
#include "../include/elba_reference.hpp"

extern "C"
{
#include <lua.h>
}

#include <cassert>

namespace elba
{

stack::stack(lua_State* L)
	: L(L)
{
}

void stack::push(const std::string& string) const
{
	lua_pushlstring(L, string.data(), string.size());
}

void stack::get(std::string& string, int index) const
{
	std::string::size_type len;
	const char* str = lua_tolstring(L, index, &len);
	string.assign(str, len);
}


void stack::push(const char* string) const
{
	lua_pushstring(L, string);
}

void stack::get(const char*& string, int index) const
{
	string = lua_tostring(L, index);
}

void stack::get(const char*& string, int index, size_t& len) const
{
	string = lua_tolstring(L, index, &len);
}


void stack::push(int integer) const
{
	lua_pushinteger(L, integer);
}

void stack::get(int& integer, int index) const
{
	integer = lua_tointeger(L, index);
}

void stack::push(long int integer) const
{
	lua_pushinteger(L, integer);
}

void stack::get(long int& integer, int index) const
{
	integer = lua_tointeger(L, index);
}


void stack::push(double number) const
{
	lua_pushnumber(L, number);
}

void stack::get(double& number, int index) const
{
	number = lua_tonumber(L, index);
}


void stack::push(float number) const
{
	lua_pushnumber(L, number);
}

void stack::get(float& number, int index) const
{
	number = lua_tonumber(L, index);
}

void stack::pop(int num) const
{
	lua_pop(L, num);
}

void stack::push(const stack::bindable_funcptr ptr, int num_upvalues) const
{
	lua_pushcclosure(L, ptr, num_upvalues);
}

void stack::push(bool boolean) const
{
	lua_pushboolean(L, boolean);
}

void stack::get(bool& boolean, int index) const
{
	boolean = lua_toboolean(L, index);
}

void stack::push(const reference& ref) const
{
	ref.push_ref();
}

void stack::get(reference& ref, int index) const
{
	lua_pushvalue(L, index);
	ref.set_ref();
}

void stack::get(stack::bindable_funcptr& ptr, int index) const
{
	ptr = lua_tocfunction(L, index);
}

int stack::size() const
{
	return lua_gettop(L);
}

bool stack::is_valid_index(int index) const
{
	return !lua_isnone(L, index);
}

int stack::normalize_index(int index) const
{
	assert(index != 0);

	if(index > 0)
		return index;

	return size() +  index - 1;
}

stack::type stack::element_type(int index) const
{
	return static_cast<stack::type>(lua_type(L, index));
}

}
