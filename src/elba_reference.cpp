#include <elba_reference.hpp>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
}

#include <cstring>

namespace elba
{

reference::reference(lua_State* L)
	: L(L)
	, ref(LUA_REFNIL)
{
}

reference::reference(lua_State* L, int index)
	: L(L)
	, ref(LUA_REFNIL)
{
	lua_pushvalue(L, index);
	set_ref();
}

reference& reference::operator=(const reference& other)
{
	other.push_ref();
	set_ref();
	return *this;
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

bool reference::operator==(const reference& other) const
{
	push_ref();
	other.push_ref();

	stack st(L);
	bool result = st.are_equal(-1, -2);

	st.pop(2);

	return result;
}

reference reference::operator()()
{
	push_ref();

	stack st(L);

	st.call(0, 1);

	reference ref(L, stack::top);

	st.pop(1);

	return ref;
}

void reference::metatable(const elba::reference& mt) const
{
	push_ref();
	mt.push_ref();

	lua_setmetatable(L, -2);
}

reference reference::metatable() const
{
	stack st(L);

	push_ref();

	if(!lua_getmetatable(L, -1))
	{
		st.pop(1);
		return reference(L);
	}

	reference mt(L, -1);
	st.pop(2);

	return mt;
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

std::ostream& operator<<(std::ostream& stream, const reference& ref)
{
	std::string str = ref;
	return stream << str;
}

std::ostream& operator<<(std::ostream& stream, const object_index& idx)
{
	std::string str = idx;
	return stream << str;
}

}
