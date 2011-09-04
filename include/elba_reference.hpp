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

class object_index;
class nil_type;

class reference
{
public:
	explicit reference(lua_State* L);
	reference(lua_State* L, int index);
	reference(const reference& other);
	~reference();

	reference& operator=(const reference& other);

	template<typename T>
	bool operator==(const T& val) const
	{
		stack st(L);
		st.push(val);
		push_ref();

		bool result = st.are_equal(-1, -2);

		st.pop(2);

		return result;
	}
	bool operator==(const reference& other) const;
	bool operator==(const nil_type& n) const { static_cast<void>(n); return type() == stack::nil; }

	template<typename T> bool operator!=(const T& val) const { return !(*this == val); }

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

		T tmp = st.get<T>(stack::top);

		st.pop(1);

		return tmp;
	}

	template<typename T> operator T() const { return get<T>(); }


	template<typename T>
	object_index operator[](const T& key) const;

	template<typename Value, typename Key>
	typename boost::disable_if_c<
		boost::is_pointer<Value>::value &&
		boost::is_same<typename boost::remove_cv<typename boost::remove_pointer<Value>::type>::type, char>::value
	, Value>::type get(const Key& key) const
	{
		stack st(L);

		push_ref();
		st.push(key);
		st.get_table_field(-2);

		Value val = st.get<Value>(stack::top);

		st.pop(1);

		return val;
	}

	template<typename T, typename U>
	void set(const T& key, const U& value) const
	{
		stack st(L);

		push_ref();
		st.push(key);
		st.push(value);
		st.set_table_field(-3);

		st.pop(1);
	}

	template<typename T, typename U>
	void raw_get(const T& key, U& value) const
	{
		stack st(L);

		push_ref();
		st.push(key);
		st.raw_get_table_field(-2);

		st.get(value, stack::top);

		st.pop(1);
	}

	template<typename T, typename U>
	void raw_set(const T& key, const U& value) const
	{
		stack st(L);

		push_ref();
		st.push(key);
		st.push(value);
		st.raw_set_table_field(-3);

		st.pop(1);
	}


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

	lua_State* const L;

	friend class object_index;
private:
	int ref;

	friend class stack;
};

std::ostream& operator<<(std::ostream& stream, const reference& ref);

class object_index
{
public:
	template<typename T>
	object_index(const reference& owner, const T& key)
		: owner_table(owner)
		, ref(owner_table.L)
	{
		ref.set_ref(key);
	}

	template<typename T>
	operator T() const
	{
		return owner_table.get<T>(ref);
	}

	template<typename T>
	const object_index& operator=(const T& value)
	{
		owner_table.set(ref, value);
		return *this;
	}

	template<typename T>
	object_index operator[](const T& key) const
	{
		reference tmp = *this;
		return tmp[key];
	}

	template<typename T>
	bool operator==(const T& val) const
	{
		return owner_table.get<reference>(ref) == val;
	}
private:
	reference owner_table;
	reference ref;
};

template<typename T>
object_index reference::operator[](const T& key) const
{
	return object_index(*this, key);
}

std::ostream& operator<<(std::ostream& stream, const object_index& idx);

}

#endif // ELBA_REFERENCE_HPP
