#ifndef ELBA_STACK_HPP
#define ELBA_STACK_HPP

#include <string>

struct lua_State;

namespace elba
{

class stack
{
public:
	enum position
	{
		top = -1,
		bottom = 1
	};
	
	enum type
	{
		none = -1,
		nil,
		boolean,
		lightuserdata,
		number,
		string,
		table,
		function,
		userdata,
		thread
	};
	
	stack(lua_State* L);
	
	template<typename T>
	T operator[](int index) const
	{
		T tmp;
		get(tmp, index);
		return tmp;
	}
	
	void push(long integer) const;
	void get(long& integer, int index = stack::top) const;

	void push(const std::string& string) const;
	void push(const char* string) const;
	void get(std::string& string, int index = stack::top) const;

	void pop(int num) const;

	int size() const;
	
	bool is_valid_index(int index) const;
	
	type element_type(int index) const;
private:
	lua_State* L;
};






}

#endif
