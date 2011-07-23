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

// START LUA FUNC GLUE
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

	template<typename T1, typename T2>
	value_ref operator()(const T1& arg1, const T2& arg2)
	{
		push_ref();

		stack st(L);

		st.push(arg1);
		st.push(arg2);

		call(2, 1);

		value_ref ref(L, stack::top);

		st.pop(1);

		return ref;
	}

	template<typename T1, typename T2, typename T3>
	value_ref operator()(const T1& arg1, const T2& arg2, const T3& arg3)
	{
		push_ref();

		stack st(L);

		st.push(arg1);
		st.push(arg2);
		st.push(arg3);

		call(3, 1);

		value_ref ref(L, stack::top);

		st.pop(1);

		return ref;
	}

	template<typename T1, typename T2, typename T3, typename T4>
	value_ref operator()(const T1& arg1, const T2& arg2, const T3& arg3, const T4& arg4)
	{
		push_ref();

		stack st(L);

		st.push(arg1);
		st.push(arg2);
		st.push(arg3);
		st.push(arg4);

		call(4, 1);

		value_ref ref(L, stack::top);

		st.pop(1);

		return ref;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5>
	value_ref operator()(const T1& arg1, const T2& arg2, const T3& arg3, const T4& arg4, const T5& arg5)
	{
		push_ref();

		stack st(L);

		st.push(arg1);
		st.push(arg2);
		st.push(arg3);
		st.push(arg4);
		st.push(arg5);

		call(5, 1);

		value_ref ref(L, stack::top);

		st.pop(1);

		return ref;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	value_ref operator()(const T1& arg1, const T2& arg2, const T3& arg3, const T4& arg4, const T5& arg5, const T6& arg6)
	{
		push_ref();

		stack st(L);

		st.push(arg1);
		st.push(arg2);
		st.push(arg3);
		st.push(arg4);
		st.push(arg5);
		st.push(arg6);

		call(6, 1);

		value_ref ref(L, stack::top);

		st.pop(1);

		return ref;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	value_ref operator()(const T1& arg1, const T2& arg2, const T3& arg3, const T4& arg4, const T5& arg5, const T6& arg6, const T7& arg7)
	{
		push_ref();

		stack st(L);

		st.push(arg1);
		st.push(arg2);
		st.push(arg3);
		st.push(arg4);
		st.push(arg5);
		st.push(arg6);
		st.push(arg7);

		call(7, 1);

		value_ref ref(L, stack::top);

		st.pop(1);

		return ref;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	value_ref operator()(const T1& arg1, const T2& arg2, const T3& arg3, const T4& arg4, const T5& arg5, const T6& arg6, const T7& arg7, const T8& arg8)
	{
		push_ref();

		stack st(L);

		st.push(arg1);
		st.push(arg2);
		st.push(arg3);
		st.push(arg4);
		st.push(arg5);
		st.push(arg6);
		st.push(arg7);
		st.push(arg8);

		call(8, 1);

		value_ref ref(L, stack::top);

		st.pop(1);

		return ref;
	}
// END LUA FUNC GLUE
private:
	lua_State* L;
	void call(int nargs, int nresults);
};

}

#endif // ELBA_FUNCTION_HPP
