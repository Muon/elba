#ifndef ELBA_STACK_HPP
#define ELBA_STACK_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <string>

struct lua_State;

namespace elba
{

class reference;
class object_index;

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

	void push(char c) const;
	void get(char& c, int index = stack::top) const;

	void push(short integer) const;
	void get(short& integer, int index = stack::top) const;

	void push(int integer) const;
	void get(int& integer, int index = stack::top) const;

	void push(long integer) const;
	void get(long& integer, int index = stack::top) const;

	void push(unsigned char c) const;
	void get(unsigned char& c, int index = stack::top) const;

	void push(unsigned short integer) const;
	void get(unsigned short& integer, int index = stack::top) const;

	void push(unsigned int integer) const;
	void get(unsigned int& integer, int index = stack::top) const;

	void push(unsigned long integer) const;
	void get(unsigned long& integer, int index = stack::top) const;

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
	
	void push(const object_index& ref) const;

	void push(void* data) const;
	void push(void*& data, size_t len) const;
	void get(void*& data, int index = stack::top) const;
	
	template<typename T>
	typename boost::disable_if<boost::is_base_of<reference, T>, void>::type push(const T& val) const
	{
		void* p;
		push(p, sizeof(T));
		*(static_cast<T*>(p)) = val;
	}
	
	template<typename T>
	void push(T* ptr) const
	{
		push(static_cast<void*>(ptr));
	}
	
	template<typename T>
	typename boost::disable_if<boost::is_base_of<reference, T>, void>::type get(T& val, int index = stack::top) const
	{
		void* p;
		get(p, index);
		
		val = *(static_cast<T*>(p));
	}

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

// START FREE FUNC GLUE
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

				st.push(function_real(arg1));

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

				function_real(arg1);

				return 0;
			}
		};

		push(reinterpret_cast<bindable_funcptr>(func_ptr));
		push(wrapper_creator::wrapper, 1);
	}

	template<typename R, typename T1, typename T2>
	void push(R (*func_ptr)(T1, T2)) const
	{
		struct wrapper_creator
		{
			typedef R (*wrapped_funcptr)(T1, T2);

			static int wrapper(lua_State* L)
			{
				stack st(L);

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get_wrapped_function());

				T1 arg1;
				st.get(arg1, 1);
				T2 arg2;
				st.get(arg2, 2);

				st.push(function_real(arg1, arg2));

				return 1;
			}
		};

		push(reinterpret_cast<bindable_funcptr>(func_ptr));
		push(wrapper_creator::wrapper, 1);
	}

	template<typename T1, typename T2>
	void push(void (*func_ptr)(T1, T2)) const
	{
		struct wrapper_creator
		{
			typedef void (*wrapped_funcptr)(T1, T2);

			static int wrapper(lua_State* L)
			{
				stack st(L);

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get_wrapped_function());

				T1 arg1;
				st.get(arg1, 1);
				T2 arg2;
				st.get(arg2, 2);

				function_real(arg1, arg2);

				return 0;
			}
		};

		push(reinterpret_cast<bindable_funcptr>(func_ptr));
		push(wrapper_creator::wrapper, 1);
	}

	template<typename R, typename T1, typename T2, typename T3>
	void push(R (*func_ptr)(T1, T2, T3)) const
	{
		struct wrapper_creator
		{
			typedef R (*wrapped_funcptr)(T1, T2, T3);

			static int wrapper(lua_State* L)
			{
				stack st(L);

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get_wrapped_function());

				T1 arg1;
				st.get(arg1, 1);
				T2 arg2;
				st.get(arg2, 2);
				T3 arg3;
				st.get(arg3, 3);

				st.push(function_real(arg1, arg2, arg3));

				return 1;
			}
		};

		push(reinterpret_cast<bindable_funcptr>(func_ptr));
		push(wrapper_creator::wrapper, 1);
	}

	template<typename T1, typename T2, typename T3>
	void push(void (*func_ptr)(T1, T2, T3)) const
	{
		struct wrapper_creator
		{
			typedef void (*wrapped_funcptr)(T1, T2, T3);

			static int wrapper(lua_State* L)
			{
				stack st(L);

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get_wrapped_function());

				T1 arg1;
				st.get(arg1, 1);
				T2 arg2;
				st.get(arg2, 2);
				T3 arg3;
				st.get(arg3, 3);

				function_real(arg1, arg2, arg3);

				return 0;
			}
		};

		push(reinterpret_cast<bindable_funcptr>(func_ptr));
		push(wrapper_creator::wrapper, 1);
	}

	template<typename R, typename T1, typename T2, typename T3, typename T4>
	void push(R (*func_ptr)(T1, T2, T3, T4)) const
	{
		struct wrapper_creator
		{
			typedef R (*wrapped_funcptr)(T1, T2, T3, T4);

			static int wrapper(lua_State* L)
			{
				stack st(L);

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get_wrapped_function());

				T1 arg1;
				st.get(arg1, 1);
				T2 arg2;
				st.get(arg2, 2);
				T3 arg3;
				st.get(arg3, 3);
				T4 arg4;
				st.get(arg4, 4);

				st.push(function_real(arg1, arg2, arg3, arg4));

				return 1;
			}
		};

		push(reinterpret_cast<bindable_funcptr>(func_ptr));
		push(wrapper_creator::wrapper, 1);
	}

	template<typename T1, typename T2, typename T3, typename T4>
	void push(void (*func_ptr)(T1, T2, T3, T4)) const
	{
		struct wrapper_creator
		{
			typedef void (*wrapped_funcptr)(T1, T2, T3, T4);

			static int wrapper(lua_State* L)
			{
				stack st(L);

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get_wrapped_function());

				T1 arg1;
				st.get(arg1, 1);
				T2 arg2;
				st.get(arg2, 2);
				T3 arg3;
				st.get(arg3, 3);
				T4 arg4;
				st.get(arg4, 4);

				function_real(arg1, arg2, arg3, arg4);

				return 0;
			}
		};

		push(reinterpret_cast<bindable_funcptr>(func_ptr));
		push(wrapper_creator::wrapper, 1);
	}

	template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
	void push(R (*func_ptr)(T1, T2, T3, T4, T5)) const
	{
		struct wrapper_creator
		{
			typedef R (*wrapped_funcptr)(T1, T2, T3, T4, T5);

			static int wrapper(lua_State* L)
			{
				stack st(L);

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get_wrapped_function());

				T1 arg1;
				st.get(arg1, 1);
				T2 arg2;
				st.get(arg2, 2);
				T3 arg3;
				st.get(arg3, 3);
				T4 arg4;
				st.get(arg4, 4);
				T5 arg5;
				st.get(arg5, 5);

				st.push(function_real(arg1, arg2, arg3, arg4, arg5));

				return 1;
			}
		};

		push(reinterpret_cast<bindable_funcptr>(func_ptr));
		push(wrapper_creator::wrapper, 1);
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5>
	void push(void (*func_ptr)(T1, T2, T3, T4, T5)) const
	{
		struct wrapper_creator
		{
			typedef void (*wrapped_funcptr)(T1, T2, T3, T4, T5);

			static int wrapper(lua_State* L)
			{
				stack st(L);

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get_wrapped_function());

				T1 arg1;
				st.get(arg1, 1);
				T2 arg2;
				st.get(arg2, 2);
				T3 arg3;
				st.get(arg3, 3);
				T4 arg4;
				st.get(arg4, 4);
				T5 arg5;
				st.get(arg5, 5);

				function_real(arg1, arg2, arg3, arg4, arg5);

				return 0;
			}
		};

		push(reinterpret_cast<bindable_funcptr>(func_ptr));
		push(wrapper_creator::wrapper, 1);
	}

	template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	void push(R (*func_ptr)(T1, T2, T3, T4, T5, T6)) const
	{
		struct wrapper_creator
		{
			typedef R (*wrapped_funcptr)(T1, T2, T3, T4, T5, T6);

			static int wrapper(lua_State* L)
			{
				stack st(L);

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get_wrapped_function());

				T1 arg1;
				st.get(arg1, 1);
				T2 arg2;
				st.get(arg2, 2);
				T3 arg3;
				st.get(arg3, 3);
				T4 arg4;
				st.get(arg4, 4);
				T5 arg5;
				st.get(arg5, 5);
				T6 arg6;
				st.get(arg6, 6);

				st.push(function_real(arg1, arg2, arg3, arg4, arg5, arg6));

				return 1;
			}
		};

		push(reinterpret_cast<bindable_funcptr>(func_ptr));
		push(wrapper_creator::wrapper, 1);
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	void push(void (*func_ptr)(T1, T2, T3, T4, T5, T6)) const
	{
		struct wrapper_creator
		{
			typedef void (*wrapped_funcptr)(T1, T2, T3, T4, T5, T6);

			static int wrapper(lua_State* L)
			{
				stack st(L);

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get_wrapped_function());

				T1 arg1;
				st.get(arg1, 1);
				T2 arg2;
				st.get(arg2, 2);
				T3 arg3;
				st.get(arg3, 3);
				T4 arg4;
				st.get(arg4, 4);
				T5 arg5;
				st.get(arg5, 5);
				T6 arg6;
				st.get(arg6, 6);

				function_real(arg1, arg2, arg3, arg4, arg5, arg6);

				return 0;
			}
		};

		push(reinterpret_cast<bindable_funcptr>(func_ptr));
		push(wrapper_creator::wrapper, 1);
	}

	template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	void push(R (*func_ptr)(T1, T2, T3, T4, T5, T6, T7)) const
	{
		struct wrapper_creator
		{
			typedef R (*wrapped_funcptr)(T1, T2, T3, T4, T5, T6, T7);

			static int wrapper(lua_State* L)
			{
				stack st(L);

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get_wrapped_function());

				T1 arg1;
				st.get(arg1, 1);
				T2 arg2;
				st.get(arg2, 2);
				T3 arg3;
				st.get(arg3, 3);
				T4 arg4;
				st.get(arg4, 4);
				T5 arg5;
				st.get(arg5, 5);
				T6 arg6;
				st.get(arg6, 6);
				T7 arg7;
				st.get(arg7, 7);

				st.push(function_real(arg1, arg2, arg3, arg4, arg5, arg6, arg7));

				return 1;
			}
		};

		push(reinterpret_cast<bindable_funcptr>(func_ptr));
		push(wrapper_creator::wrapper, 1);
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	void push(void (*func_ptr)(T1, T2, T3, T4, T5, T6, T7)) const
	{
		struct wrapper_creator
		{
			typedef void (*wrapped_funcptr)(T1, T2, T3, T4, T5, T6, T7);

			static int wrapper(lua_State* L)
			{
				stack st(L);

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get_wrapped_function());

				T1 arg1;
				st.get(arg1, 1);
				T2 arg2;
				st.get(arg2, 2);
				T3 arg3;
				st.get(arg3, 3);
				T4 arg4;
				st.get(arg4, 4);
				T5 arg5;
				st.get(arg5, 5);
				T6 arg6;
				st.get(arg6, 6);
				T7 arg7;
				st.get(arg7, 7);

				function_real(arg1, arg2, arg3, arg4, arg5, arg6, arg7);

				return 0;
			}
		};

		push(reinterpret_cast<bindable_funcptr>(func_ptr));
		push(wrapper_creator::wrapper, 1);
	}

	template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	void push(R (*func_ptr)(T1, T2, T3, T4, T5, T6, T7, T8)) const
	{
		struct wrapper_creator
		{
			typedef R (*wrapped_funcptr)(T1, T2, T3, T4, T5, T6, T7, T8);

			static int wrapper(lua_State* L)
			{
				stack st(L);

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get_wrapped_function());

				T1 arg1;
				st.get(arg1, 1);
				T2 arg2;
				st.get(arg2, 2);
				T3 arg3;
				st.get(arg3, 3);
				T4 arg4;
				st.get(arg4, 4);
				T5 arg5;
				st.get(arg5, 5);
				T6 arg6;
				st.get(arg6, 6);
				T7 arg7;
				st.get(arg7, 7);
				T8 arg8;
				st.get(arg8, 8);

				st.push(function_real(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8));

				return 1;
			}
		};

		push(reinterpret_cast<bindable_funcptr>(func_ptr));
		push(wrapper_creator::wrapper, 1);
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	void push(void (*func_ptr)(T1, T2, T3, T4, T5, T6, T7, T8)) const
	{
		struct wrapper_creator
		{
			typedef void (*wrapped_funcptr)(T1, T2, T3, T4, T5, T6, T7, T8);

			static int wrapper(lua_State* L)
			{
				stack st(L);

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get_wrapped_function());

				T1 arg1;
				st.get(arg1, 1);
				T2 arg2;
				st.get(arg2, 2);
				T3 arg3;
				st.get(arg3, 3);
				T4 arg4;
				st.get(arg4, 4);
				T5 arg5;
				st.get(arg5, 5);
				T6 arg6;
				st.get(arg6, 6);
				T7 arg7;
				st.get(arg7, 7);
				T8 arg8;
				st.get(arg8, 8);

				function_real(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);

				return 0;
			}
		};

		push(reinterpret_cast<bindable_funcptr>(func_ptr));
		push(wrapper_creator::wrapper, 1);
	}
// END FREE FUNC GLUE

	bindable_funcptr get_wrapped_function() const;

	void pop(int num) const;

	int size() const;
	bool is_valid_index(int index) const;
	int normalize_index(int index) const;
	int upvalue_index(int index) const;

	type element_type(int index) const;
private:
	lua_State* L;
};

}

#endif
