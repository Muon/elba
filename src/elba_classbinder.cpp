#include <elba_classbinder.hpp>

namespace elba
{

class_binder::class_binder(lua_State* L)
	: L(L)
	, metatable(make_table(L))
	, methods(make_table(L))
{
	metatable.set("__index", methods);
}

template<>
void stack::push<class_binder>(const class_binder& binder) const
{
	push(binder.methods);
}

}
