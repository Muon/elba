#ifndef ELBA_TABLE_HPP
#define ELBA_TABLE_HPP

#include "elba_stack.hpp"
#include "elba_reference.hpp"

struct lua_State;

namespace elba
{

class table : public reference
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
			owner_table.set(key, value);
			return *this;
		}
	private:
		const table& owner_table;
		const T& key;
		
		template<typename U>
		U convert()
		{
			U tmp;
			owner_table.get(key, tmp);
			return tmp;
		}
	};

	template <typename T>
	index<T> operator[](const T& key) const
	{
		return index<T>(*this, key);
	}
	
	template<typename T, typename U>
	void get(const T& key, U& value) const
	{
		cur_stack.push(key);
		get_table_field();

		cur_stack.get(value, stack::top);

		cur_stack.pop(1);
	}
	
	template<typename T, typename U>
	void set(const T& key, const U& value) const
	{
		cur_stack.push(key);
		cur_stack.push(value);
		set_table_field();
	}
private:
	void get_table_field() const;
	void set_table_field() const;

	lua_State* L;
	
	stack cur_stack;
	
	template <typename T>
	friend class index;
};

}

#endif
