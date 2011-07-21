#include "elba_valueref.hpp"

#include <cstring>

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

template<> value_ref::operator const char*()
{
	stack st(L);

	push_ref();

	const char* tmp;
	size_t len;
	st.get(tmp, stack::top, len);

	char* str = new char[len + 1];
	std::memcpy(str, tmp, len + 1);

	st.pop(1);

	return str;
}

}
