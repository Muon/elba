#ifndef ELBA_STACK_HPP
#define ELBA_STACK_HPP

#include <string>

struct lua_State;

namespace elba
{

class reference;

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

	void push(float number) const;
	void get(float& number, int index = stack::top) const;

	void push(const std::string& string) const;
	void get(std::string& string, int index = stack::top) const;

	void push(const char* string) const;
	void get(const char*& string, int index = stack::top) const;
	void get(const char*& string, int index, size_t& len) const;

	void push(const bindable_funcptr ptr, int num_upvalues = 0) const;
	void get(bindable_funcptr& ptr, int index = stack::top) const;

	void push(bool boolean) const;
	void get(bool& boolean, int index = stack::top) const;

	void push(const reference& ref) const;
	void get(reference& ref, int index = stack::top) const;

	void push(void (*func_ptr)()) const;

	template<typename R>
	void push(R (*func_ptr)()) const
	{
		struct wrapper_creator
		{
			typedef R (*wrapped_funcptr)();

			static int wrapper(lua_State* L)
			{
				stack st(L);

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get_wrapped_function());

				st.push(function_real());

				return 1;
			}
		};

		push(reinterpret_cast<bindable_funcptr>(func_ptr));
		push(wrapper_creator::wrapper, 1);
	}

	template<typename T1>
	void push(void (*func_ptr)(T1)) const
	{
		struct wrapper_creator
		{
			typedef void (*wrapped_funcptr)(T1);

			static int wrapper(lua_State* L)
			{
				stack st(L);

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get_wrapped_function());

				T1 arg1;
				st.get(arg1, 1);

				st.pop(1);

				function_real(arg1);

				return 0;
			}
		};

		push(reinterpret_cast<bindable_funcptr>(func_ptr));
		push(wrapper_creator::wrapper, 1);
	}


	template<typename R, typename T1>
	void push(R (*func_ptr)(T1)) const
	{
		struct wrapper_creator
		{
			typedef R (*wrapped_funcptr)(T1);

			static int wrapper(lua_State* L)
			{
				stack st(L);

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get_wrapped_function());

				T1 arg1;
				st.get(arg1, 1);

				st.pop(1);

				st.push(function_real(arg1));

				return 1;
			}
		};

		push(reinterpret_cast<bindable_funcptr>(func_ptr));
		push(wrapper_creator::wrapper, 1);
	}

	bindable_funcptr get_wrapped_function() const;

	void pop(int num) const;

	int size() const;
	bool is_valid_index(int index) const;
	int normalize_index(int index) const;

	type element_type(int index) const;
private:
	lua_State* L;
};

}

#endif
