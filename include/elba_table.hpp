#ifndef ELBA_TABLE_HPP
#define ELBA_TABLE_HPP

#include "elba_stack.hpp"
#include "elba_reference.hpp"

#include <iosfwd>

struct lua_State;

namespace elba
{

class object_index;

class table : public reference
{
public:
	table();
	table(lua_State* L);
	table(lua_State* L, int index);
	table(lua_State* L, int num_array, int num_assoc);

	template<typename T>
	object_index operator[](const T& key) const;

	template<typename T, typename U>
	typename boost::disable_if_c<
		boost::is_pointer<U>::value &&
		boost::is_same<typename boost::remove_cv<typename boost::remove_pointer<U>::type>::type, char>::value
	, void>::type get(const T& key, U& value) const
	{
		stack st(L);

		push_ref();
		st.push(key);
		st.get_table_field(-2);

		st.get(value, stack::top);

		st.pop(1);
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
private:
	friend class object_index;
};

class object_index
{
public:
	template<typename T>
	object_index(const table& owner, const T& key)
		: owner_table(owner)
		, ref(owner_table.L)
	{
		ref.set_ref(key);
	}

	template<typename T>
	operator T() const
	{
		T tmp;
		owner_table.get(ref, tmp);

		return tmp;
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
		table tmp = *this;
		return tmp[key];
	}
private:
	table owner_table;
	reference ref;
};

template<typename T>
object_index table::operator[](const T& key) const
{
	return object_index(*this, key);
}

std::ostream& operator<<(std::ostream& stream, const object_index& idx);

}

#endif
