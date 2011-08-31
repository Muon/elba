#ifndef ELBA_REFERENCE_HPP
#define ELBA_REFERENCE_HPP

#include "elba_stack.hpp"

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_pointer.hpp>

struct lua_State;

namespace elba
{

class reference
{
public:
	reference();
	reference(lua_State* L);
	reference(lua_State* L, int index);
	reference(const reference& other);
	~reference();

	reference& operator=(const reference& other);

	template<typename T>
	reference& set_ref(const T& val)
	{
		stack st(L);
		st.push(val);
		set_ref();

		return *this;
	}

	template<typename T>
	typename boost::disable_if_c<
		boost::is_pointer<T>::value &&
		boost::is_same<typename boost::remove_cv<typename boost::remove_pointer<T>::type>::type, char>::value
	, T>::type get() const
	{
		stack st(L);

		push_ref();

		T tmp;
		st.get(tmp, stack::top);

		st.pop(1);

		return tmp;
	}

	template<typename T> operator T() const { return get<T>(); }

	reference operator()();

// START LUA FUNC GLUE
	template<typename T1>
	reference operator()(const T1& arg1)
	{
		push_ref();

		stack st(L);

		st.push(arg1);

		st.call(1, 1);

		reference ref(L, stack::top);

		st.pop(1);

		return ref;
	}

	template<typename T1, typename T2>
	reference operator()(const T1& arg1, const T2& arg2)
	{
		push_ref();

		stack st(L);

		st.push(arg1);
		st.push(arg2);

		st.call(2, 1);

		reference ref(L, stack::top);

		st.pop(1);

		return ref;
	}

	template<typename T1, typename T2, typename T3>
	reference operator()(const T1& arg1, const T2& arg2, const T3& arg3)
	{
		push_ref();

		stack st(L);

		st.push(arg1);
		st.push(arg2);
		st.push(arg3);

		st.call(3, 1);

		reference ref(L, stack::top);

		st.pop(1);

		return ref;
	}

	template<typename T1, typename T2, typename T3, typename T4>
	reference operator()(const T1& arg1, const T2& arg2, const T3& arg3, const T4& arg4)
	{
		push_ref();

		stack st(L);

		st.push(arg1);
		st.push(arg2);
		st.push(arg3);
		st.push(arg4);

		st.call(4, 1);

		reference ref(L, stack::top);

		st.pop(1);

		return ref;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5>
	reference operator()(const T1& arg1, const T2& arg2, const T3& arg3, const T4& arg4, const T5& arg5)
	{
		push_ref();

		stack st(L);

		st.push(arg1);
		st.push(arg2);
		st.push(arg3);
		st.push(arg4);
		st.push(arg5);

		st.call(5, 1);

		reference ref(L, stack::top);

		st.pop(1);

		return ref;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	reference operator()(const T1& arg1, const T2& arg2, const T3& arg3, const T4& arg4, const T5& arg5, const T6& arg6)
	{
		push_ref();

		stack st(L);

		st.push(arg1);
		st.push(arg2);
		st.push(arg3);
		st.push(arg4);
		st.push(arg5);
		st.push(arg6);

		st.call(6, 1);

		reference ref(L, stack::top);

		st.pop(1);

		return ref;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	reference operator()(const T1& arg1, const T2& arg2, const T3& arg3, const T4& arg4, const T5& arg5, const T6& arg6, const T7& arg7)
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

		st.call(7, 1);

		reference ref(L, stack::top);

		st.pop(1);

		return ref;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	reference operator()(const T1& arg1, const T2& arg2, const T3& arg3, const T4& arg4, const T5& arg5, const T6& arg6, const T7& arg7, const T8& arg8)
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

		st.call(8, 1);

		reference ref(L, stack::top);

		st.pop(1);

		return ref;
	}
// END LUA FUNC GLUE

	void metatable(const reference& mt) const;
	reference metatable() const;

	stack::type type() const
	{
		stack st(L);

		push_ref();

		stack::type t = st.element_type(stack::top);

		st.pop(1);

		return t;
	}

protected:
	void set_ref();
	void push_ref() const;

	lua_State* L;
private:
	int ref;

	friend class stack;
};

}

#endif // ELBA_REFERENCE_HPP
