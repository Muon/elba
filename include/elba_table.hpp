#ifndef ELBA_TABLE_HPP
#define ELBA_TABLE_HPP

#include "elba_stack.hpp"
#include "elba_reference.hpp"
#include "elba_function.hpp"

#include <iosfwd>

struct lua_State;

namespace elba
{

class table : public reference
{
public:
	table();
	table(lua_State* L);
	table(lua_State* L, int index);
	table(lua_State* L, int num_array, int num_assoc);

	template<typename T>
	class index;

	template <typename T>
	index<T> operator[](const T& key) const
	{
		return index<T>(*this, key);
	}

	template<typename T, typename U>
	void get(const T& key, U& value) const
	{
		stack st(L);

		push_ref();
		st.push(key);
		get_table_field(-2);

		get_top(value);

		st.pop(1);
	}

	template<typename T, typename U>
	void set(const T& key, const U& value) const
	{
		stack st(L);

		push_ref();
		st.push(key);
		st.push(value);
		set_table_field(-3);

		st.pop(1);
	}

	void set_metatable(const table& t) const;
	table get_metatable() const;
private:
	void get_table_field(int index) const;
	void set_table_field(int index) const;

	template <typename T>
	friend class index;

	template<typename T>
	void get_top(T& val) const
	{
		stack st(L);
		st.get(val, stack::top);
	}
};

template<> void table::get_top<const char*>(const char*& str) const;

template<typename T>
class table::index
{
public:
	index(const table& owner, const T& key)
		: owner_table(owner)
		, key(key)
	{
	}

	template<typename U>
	operator U() const
	{
		U tmp;
		owner_table.get(key, tmp);

		return tmp;
	}

	template<typename U>
	const index& operator=(const U& value) const
	{
		owner_table.set(key, value);
		return *this;
	}

	template<typename U>
	index<U> operator[](const U& key) const
	{
		table tmp = *this;
		return tmp[key];
	}
private:
	table owner_table;
	const T& key;
};

template<typename T>
std::ostream& operator<<(std::ostream& stream, const table::index<T>& idx)
{
	std::string str = idx;
	stream << str;
	return stream;
}

}

#endif
