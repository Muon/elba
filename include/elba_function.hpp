#ifndef ELBA_FUNCTION_HPP
#define ELBA_FUNCTION_HPP

#include "elba_reference.hpp"
#include "elba_stack.hpp"
#include "elba_valueref.hpp"

namespace elba
{

class function : public reference
{
public:
	function(lua_State* L);
	function(lua_State* L, int index);

	template<typename T1>
	value_ref operator()(const T1& arg1)
	{
		push_ref();

		stack st(L);
		st.push(arg1);

		call(1, 1);

		value_ref ref(L, stack::top);

		st.pop(1);

		return ref;
	}
private:
	lua_State* L;
	void call(int nargs, int nresults);
};

}

#endif // ELBA_FUNCTION_HPP
