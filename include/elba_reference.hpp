#ifndef ELBA_REFERENCE_HPP
#define ELBA_REFERENCE_HPP

#include "elba_stack.hpp"
#include "elba_nil.hpp"

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_pointer.hpp>

struct lua_State;

namespace elba
{

class object_index;
class reference;

template<> void stack::push<reference>(const reference& ref) const;
template<> reference stack::get<reference>(int idx) const;

bool operator==(const reference& val, const nil_type&);
inline bool operator==(const nil_type& n, const reference& val) { return val == n; }
inline bool operator!=(const reference& val, const nil_type& n) { return !(val == n); }
inline bool operator!=(const nil_type& n, const reference& val) { return !(val == n); }

class reference
{
public:
	explicit reference(lua_State* L);
	reference(lua_State* L, int index);
	reference(const reference& other);
	~reference();

	void swap(reference& other);

	reference& operator=(elba::reference other);
	template<typename T>
	reference& operator=(const T& val)
	{
		stack st(L);
		st.push(val);
		set_ref();

		return *this;
	}

	template<typename T>
	bool operator==(const T& val) const
	{
		stack st(L);
		st.push(val);
		st.push(*this);

		bool result = st.are_equal(-1, -2);

		st.pop(2);

		return result;
	}
	bool operator==(const reference& other) const;

	template<typename T> bool operator!=(const T& val) const { return !(*this == val); }

	template<typename T>
	typename boost::disable_if_c<
		boost::is_pointer<T>::value &&
		boost::is_same<typename boost::remove_cv<typename boost::remove_pointer<T>::type>::type, char>::value
	, T>::type get() const
	{
		stack st(L);

		st.push(*this);

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

		st.push(*this);
		st.get_table_field(-1, key);

		Value val = st.get<Value>(stack::top);

		st.pop(2);

		return val;
	}

	template<typename T, typename U>
	void set(const T& key, const U& value) const
	{
		stack st(L);

		st.push(*this);
		st.set_table_field(-1, key, value);

		st.pop(1);
	}

	template<typename Value, typename Key>
	typename boost::disable_if_c<
		boost::is_pointer<Value>::value &&
		boost::is_same<typename boost::remove_cv<typename boost::remove_pointer<Value>::type>::type, char>::value
	, Value>::type raw_get(const Key& key) const
	{
		stack st(L);

		st.push(*this);
		st.raw_get_table_field(-1, key);

		Value val = st.get<Value>(stack::top);

		st.pop(2);

		return val;
	}

	template<typename T, typename U>
	void raw_set(const T& key, const U& value) const
	{
		stack st(L);

		st.push(*this);
		st.raw_set_table_field(-1, key, value);

		st.pop(1);
	}

#include "elba_lua_functions.inc.hpp"

	void metatable(const reference& mt) const;
	reference metatable() const;

	stack::type type() const
	{
		stack st(L);

		st.push(*this);

		stack::type t = st.element_type(stack::top);

		st.pop(1);

		return t;
	}

private:
	void set_ref();

	lua_State* L;

	int ref;

	// FIXME: Why can't I make this more specific on GCC?
	template<typename T> friend void stack::push(const T& ref) const;
	friend bool operator==(const reference& val, const nil_type&);
};

std::ostream& operator<<(std::ostream& stream, const reference& ref);

inline reference make_table(lua_State* L)
{
	stack st(L);
	st.create_table();
	reference ref(L, -1);
	st.pop(1);

	return ref;
}

inline reference make_userdata(lua_State* L, std::size_t size)
{
	stack st(L);
	st.create_userdata(size);
	reference ref(L, -1);
	st.pop(1);

	return ref;
}

}

namespace std { template<> inline void swap<elba::reference>(elba::reference& a, elba::reference& b) { a.swap(b); } }

namespace elba
{

template<> void stack::push<object_index>(const object_index& idx) const;

class object_index
{
public:
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

#include "elba_lua_functions_index.inc.hpp"

private:
	object_index(const reference& owner, const reference& key)
	: owner_table(owner)
	, ref(key)
	{
	}

	reference owner_table;
	reference ref;

	// FIXME: Why can't I make this more specific on GCC?
	template<typename T> friend void stack::push(const T& idx) const;
	friend class reference;
};

template<typename T>
object_index reference::operator[](const T& key) const
{
	reference k(L);
	k = key;

	return object_index(*this, k);
}

std::ostream& operator<<(std::ostream& stream, const object_index& idx);

}

#endif // ELBA_REFERENCE_HPP
