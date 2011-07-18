#include <elba_reference.hpp>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
}

namespace elba
{

reference& reference::operator=(const reference& other)
{
	L = other.L;
	other.push_ref();
	set_ref();
	return *this;
}

reference::reference()
	: L(NULL)
	, ref(LUA_REFNIL)
{
}

reference::reference(lua_State* L)
	: L(L)
	, ref(LUA_REFNIL)
{
}

reference::reference(const reference& other)
	: L(other.L)
	, ref(LUA_REFNIL)
{
	*this = other;
}

reference::~reference()
{
	luaL_unref(L, LUA_REGISTRYINDEX, ref);
}

void reference::set_ref()
{
	luaL_unref(L, LUA_REGISTRYINDEX, ref);

	ref = luaL_ref(L, LUA_REGISTRYINDEX);
}

void reference::push_ref() const
{
	lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
}

}