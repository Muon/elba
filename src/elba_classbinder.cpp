#include <elba_classbinder.hpp>

namespace elba
{

class_binder::class_binder(lua_State* L)
	: metatable(L)
	, methods(L)
	, L(L)
{
	metatable["__index"] = methods;
}

}