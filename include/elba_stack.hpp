#ifndef ELBA_STACK_HPP
#define ELBA_STACK_HPP

#include <string>

struct lua_State;

namespace elba
{

class stack
{
public:
	typedef int(*bindable_funcptr)(lua_State*);
	
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
	
	void push(int integer) const;
	void get(int& integer, int index = stack::top) const;
	
	void push(long integer) const;
	void get(long& integer, int index = stack::top) const;
	
	void push(double number) const;
	void get(double& number, int index = stack::top) const;

	void push(const std::string& string) const;
	void get(std::string& string, int index = stack::top) const;
	
	void push(const char* string) const;
	void get(const char*& string, int index = stack::top) const;
	
	void push(bindable_funcptr ptr, int num_upvalues = 0) const;
	void get(bindable_funcptr& ptr, int index = stack::top) const;
	
	template<typename R, typename T1>
	void push(R (*func_ptr)(T1)) const
	{
		typedef R (*wrapped_funcptr)(T1);
		
		struct wrapper_creator
		{
			static int wrapper(lua_State* L)
			{
				stack st(L);
				
				if(st.size() != 1)
					throw;
				
				bindable_funcptr function;
				st.get(function, -10002 - 1);
				
				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(function);
				
				
				T1 arg1;
				st.get(arg1, 1);
				
				st.pop(1);
				
				st.push(function_real(arg1));
				
				return 1;
			}
		};
		
		push(reinterpret_cast<bindable_funcptr>(func_ptr));
		push(&wrapper_creator::wrapper, 1);
	}

	void pop(int num) const;

	int size() const;
	
	bool is_valid_index(int index) const;
	
	type element_type(int index) const;
private:
	lua_State* L;
};

}

#endif
