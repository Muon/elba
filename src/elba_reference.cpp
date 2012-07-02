#include <elba_reference.hpp>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
}

#include <algorithm>
#include <cstring>
#include <cassert>

namespace elba
{

template<>
void stack::push<reference>(const reference& ref) const
{
	lua_rawgeti(L, LUA_REGISTRYINDEX, ref.ref);
}

template<>
reference stack::get<reference>(int idx) const
{
	return reference(L, idx);
}

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

reference::reference(const reference& other)
	: L(other.L)
	, ref(LUA_REFNIL)
{
	stack st(L);
	st.push(other);
	set_ref();
}

reference::~reference()
{
	luaL_unref(L, LUA_REGISTRYINDEX, ref);
}

reference& reference::operator=(reference other)
{
	swap(other);
	return *this;
}

bool reference::equals(const nil_type&) const
{
	return ref == LUA_REFNIL;
}

void reference::swap(reference& other)
{
	std::swap(L, other.L);
	std::swap(ref, other.ref);
}

void reference::metatable(const elba::reference& mt) const
{
	stack st(L);
	st.push(*this);
	st.push(mt);
	st.set_metatable(-2);
	st.pop(1);
}

reference reference::metatable() const
{
	stack st(L);

	st.push(*this);

	if(!st.get_metatable(-1))
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
	if(ref != LUA_REFNIL && ref != LUA_NOREF)
	{
		lua_rawseti(L, LUA_REGISTRYINDEX, ref);
	}
	else
	{
		ref = luaL_ref(L, LUA_REGISTRYINDEX);
	}
}

std::ostream& operator<<(std::ostream& stream, const reference& ref)
{
	std::string str = ref;
	return stream << str;
}

template<>
void stack::push<object_index>(const object_index& idx) const
{
	push(idx.operator reference());
}

std::ostream& operator<<(std::ostream& stream, const object_index& idx)
{
	std::string str = idx;
	return stream << str;
}


}
