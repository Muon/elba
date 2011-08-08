#ifndef ELBA_TABLE_HPP
#define ELBA_TABLE_HPP

#include "elba_stack.hpp"
#include "elba_reference.hpp"
#include "elba_function.hpp"

#include <iosfwd>

struct lua_State;

namespace elba
{

class index;

class table : public reference
{
public:
	table();
	table(lua_State* L);
	table(lua_State* L, int index);
	table(lua_State* L, int num_array, int num_assoc);

	template<typename T>
	index operator[](const T& key) const;

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

	friend class index;

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

class index
{
public:
	template<typename T>
	index(const table& owner, const T& key)
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
	const index& operator=(const U& value)
	{
		owner_table.set(ref, value);
		return *this;
	}

	template<typename U>
	index operator[](const U& key) const
	{
		table tmp = *this;
		return tmp[key];
	}
private:
	table owner_table;
	reference ref;
};

template<typename T>
index table::operator[](const T& key) const
{
	return index(*this, key);
}

template<> void table::get_top<char*>(char*& str) const;
template<> void table::get_top<const char*>(const char*& str) const;

std::ostream& operator<<(std::ostream& stream, const index& idx);

}

#endif
