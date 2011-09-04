#include <elba_classbinder.hpp>

namespace elba
{

class_binder::class_binder(lua_State* L)
	: metatable(L)
	, methods(L)
	, L(L)
{
	stack st(L);
	metatable = st.create_table();
	metatable["__index"] = st.create_table();
	methods = metatable["__index"];
}

}
