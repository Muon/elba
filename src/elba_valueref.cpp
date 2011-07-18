#include "elba_valueref.hpp"

extern "C"
{
#include <lua.h>
}

namespace elba
{

value_ref::value_ref(lua_State* L, int index)
	: reference(L)
{
	lua_pushvalue(L, index);
	set_ref();
}

template<> value_ref::operator std::string() { return get<std::string>(); }

}