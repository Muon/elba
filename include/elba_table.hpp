#ifndef ELBA_TABLE_HPP
#define ELBA_TABLE_HPP

#include "elba_stack.hpp"

struct lua_State;

namespace elba
{

class table
{
public:
	table(lua_State* L);
	table(lua_State* L, int index);
	table(lua_State* L, int num_array, int num_assoc);

	template<typename T>
	class index
	{
	public:
		index(const table& owner, const T& key)
			: owner_table(owner)
			, key(key)
		{
		}
		
		operator std::string()
		{
			return convert<std::string>();
		}
		
		operator int()
		{
			return convert<int>();
		}
		
		operator double()
		{
			return convert<double>();
		}
		
		operator float()
		{
			return convert<float>();
		}
		
		operator bool()
		{
			return convert<bool>();
		}
		
		template<typename U>
		index& operator=(const U& value)
		{
			const stack& st = owner_table.cur_stack;
			st.push(key);
			st.push(value);
			
			owner_table.set_table_field();
			
			return *this;
		}
	private:
		const table& owner_table;
		const T& key;
		
		template<typename U>
		U convert()
		{
			const stack& st = owner_table.cur_stack;
			st.push(key);
			owner_table.get_table_field();

			U value;
			st.get(value, stack::top);

			st.pop(1);
			
			return value;
		}
	};

	template <typename T>
	index<T> operator[](const T& key) const
	{
		return index<T>(*this, key);
	}
private:
	void get_table_field() const;
	void set_table_field() const;

	lua_State* L;
	
	int stack_index;
	stack cur_stack;
	
	template <typename T>
	friend class index;
};

}

#endif
