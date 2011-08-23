#ifndef ELBA_TABLE_HPP
#define ELBA_TABLE_HPP

#include "elba_stack.hpp"
#include "elba_reference.hpp"
#include "elba_function.hpp"

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
	void get(const T& key, U& value) const
	{
		stack st(L);

		push_ref();
		st.push(key);
		st.get_table_field(-2);

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
		st.set_table_field(-3);

		st.pop(1);
	}

private:
	friend class object_index;

	template<typename T>
	void get_top(T& val) const
	{
		stack st(L);
		st.get(val, stack::top);
	}

	template<typename T>
	void get_top(T*& val) const
	{
		stack st(L);
		void* p;
		st.get(p, stack::top);
		val = static_cast<T*>(p);
	}

	template<typename T>
	void get_top(const T*& val) const
	{
		stack st(L);
		void* p;
		st.get(p, stack::top);
		val = static_cast<T*>(p);
	}
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

	template<typename U>
	operator U() const
	{
		U tmp;
		owner_table.get(ref, tmp);

		return tmp;
	}

	template<typename U>
	const object_index& operator=(const U& value)
	{
		owner_table.set(ref, value);
		return *this;
	}

	template<typename U>
	object_index operator[](const U& key) const
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

template<> void table::get_top<char*>(char*& str) const;
template<> void table::get_top<const char*>(const char*& str) const;

std::ostream& operator<<(std::ostream& stream, const object_index& idx);

}

#endif
