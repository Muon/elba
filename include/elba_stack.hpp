#ifndef ELBA_STACK_HPP
#define ELBA_STACK_HPP

#include <string>

struct lua_State;

namespace elba
{

class reference;
class object_index;
class class_binder;

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
	void get(const char*& string, int index, std::size_t& len) const;

	void push(const bindable_funcptr ptr, int num_upvalues = 0) const;
	void get(bindable_funcptr& ptr, int index = stack::top) const;

	void push(bool boolean) const;
	void get(bool& boolean, int index = stack::top) const;

	void push(const reference& ref) const;
	void get(reference& ref, int index = stack::top) const;

	void push(const object_index& ref) const;

	void push(void* data) const;
	void get(void*& data, int index = stack::top) const;

	void push(const class_binder& binder) const;

	template<typename T>
	void push(const T& val) const
	{
		reference ud = create_userdata(sizeof(T));
		void* p = ud;
		*(static_cast<T*>(p)) = val;

		push(ud);
	}

	template<typename T>
	void push(T* ptr) const
	{
		push(static_cast<void*>(ptr));
	}

	template<typename T>
	void get(T& val, int index = stack::top) const
	{
		void* p;
		get(p, index);

		val = *(static_cast<T*>(p));
	}

	template<typename T>
	void get(T*& val, int index = stack::top) const
	{
		void* p;
		get(p, index);

		val = static_cast<T*>(p);
	}

	template<typename R, typename C>
	void push(R (C::*func_ptr)()) const
	{
		typedef R (C::*memfunptr)() const;
		struct wrapper_creator
		{
			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)());

				return 1;
			}
		};

		push_memfun_wrapper(&wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C>
	void push(R (C::*func_ptr)() const) const
	{
		typedef R (C::*memfunptr)() const;
		struct wrapper_creator
		{
			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)());

				return 1;
			}
		};

		push_memfun_wrapper(&wrapper_creator::wrapper, func_ptr);
	}

	template<typename C>
	void push(void (C::*func_ptr)()) const
	{
		typedef void (C::*memfunptr)() const;
		struct wrapper_creator
		{
			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)());

				return 1;
			}
		};

		push_memfun_wrapper(&wrapper_creator::wrapper, func_ptr);
	}

	template<typename C>
	void push(void (C::*func_ptr)() const) const
	{
		typedef void (C::*memfunptr)() const;
		struct wrapper_creator
		{
			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)());

				return 1;
			}
		};

		push_memfun_wrapper(&wrapper_creator::wrapper, func_ptr);
	}

// START MEM FUNC GLUE
	template<typename R, typename C, typename T1>
	void push(R (C::*func_ptr)(T1) ) const
	{
		struct wrapper_creator
		{
			typedef R (C::*memfunptr)(T1) ;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1>
	void push(R (C::*func_ptr)(T1) const) const
	{
		struct wrapper_creator
		{
			typedef R (C::*memfunptr)(T1) const;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1>
	void push(void (C::*func_ptr)(T1) ) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1) ;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1>
	void push(void (C::*func_ptr)(T1) const) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1) const;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2>
	void push(R (C::*func_ptr)(T1, T2) ) const
	{
		struct wrapper_creator
		{
			typedef R (C::*memfunptr)(T1, T2) ;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2>
	void push(R (C::*func_ptr)(T1, T2) const) const
	{
		struct wrapper_creator
		{
			typedef R (C::*memfunptr)(T1, T2) const;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2>
	void push(void (C::*func_ptr)(T1, T2) ) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2) ;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2>
	void push(void (C::*func_ptr)(T1, T2) const) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2) const;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2, typename T3>
	void push(R (C::*func_ptr)(T1, T2, T3) ) const
	{
		struct wrapper_creator
		{
			typedef R (C::*memfunptr)(T1, T2, T3) ;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);
				T3 arg3;
				st.get(arg3, 4);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2, arg3));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2, typename T3>
	void push(R (C::*func_ptr)(T1, T2, T3) const) const
	{
		struct wrapper_creator
		{
			typedef R (C::*memfunptr)(T1, T2, T3) const;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);
				T3 arg3;
				st.get(arg3, 4);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2, arg3));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2, typename T3>
	void push(void (C::*func_ptr)(T1, T2, T3) ) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2, T3) ;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);
				T3 arg3;
				st.get(arg3, 4);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2, arg3));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2, typename T3>
	void push(void (C::*func_ptr)(T1, T2, T3) const) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2, T3) const;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);
				T3 arg3;
				st.get(arg3, 4);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2, arg3));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2, typename T3, typename T4>
	void push(R (C::*func_ptr)(T1, T2, T3, T4) ) const
	{
		struct wrapper_creator
		{
			typedef R (C::*memfunptr)(T1, T2, T3, T4) ;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);
				T3 arg3;
				st.get(arg3, 4);
				T4 arg4;
				st.get(arg4, 5);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2, arg3, arg4));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2, typename T3, typename T4>
	void push(R (C::*func_ptr)(T1, T2, T3, T4) const) const
	{
		struct wrapper_creator
		{
			typedef R (C::*memfunptr)(T1, T2, T3, T4) const;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);
				T3 arg3;
				st.get(arg3, 4);
				T4 arg4;
				st.get(arg4, 5);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2, arg3, arg4));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2, typename T3, typename T4>
	void push(void (C::*func_ptr)(T1, T2, T3, T4) ) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2, T3, T4) ;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);
				T3 arg3;
				st.get(arg3, 4);
				T4 arg4;
				st.get(arg4, 5);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2, arg3, arg4));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2, typename T3, typename T4>
	void push(void (C::*func_ptr)(T1, T2, T3, T4) const) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2, T3, T4) const;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);
				T3 arg3;
				st.get(arg3, 4);
				T4 arg4;
				st.get(arg4, 5);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2, arg3, arg4));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2, typename T3, typename T4, typename T5>
	void push(R (C::*func_ptr)(T1, T2, T3, T4, T5) ) const
	{
		struct wrapper_creator
		{
			typedef R (C::*memfunptr)(T1, T2, T3, T4, T5) ;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);
				T3 arg3;
				st.get(arg3, 4);
				T4 arg4;
				st.get(arg4, 5);
				T5 arg5;
				st.get(arg5, 6);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2, arg3, arg4, arg5));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2, typename T3, typename T4, typename T5>
	void push(R (C::*func_ptr)(T1, T2, T3, T4, T5) const) const
	{
		struct wrapper_creator
		{
			typedef R (C::*memfunptr)(T1, T2, T3, T4, T5) const;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);
				T3 arg3;
				st.get(arg3, 4);
				T4 arg4;
				st.get(arg4, 5);
				T5 arg5;
				st.get(arg5, 6);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2, arg3, arg4, arg5));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2, typename T3, typename T4, typename T5>
	void push(void (C::*func_ptr)(T1, T2, T3, T4, T5) ) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2, T3, T4, T5) ;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);
				T3 arg3;
				st.get(arg3, 4);
				T4 arg4;
				st.get(arg4, 5);
				T5 arg5;
				st.get(arg5, 6);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2, arg3, arg4, arg5));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2, typename T3, typename T4, typename T5>
	void push(void (C::*func_ptr)(T1, T2, T3, T4, T5) const) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2, T3, T4, T5) const;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);
				T3 arg3;
				st.get(arg3, 4);
				T4 arg4;
				st.get(arg4, 5);
				T5 arg5;
				st.get(arg5, 6);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2, arg3, arg4, arg5));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	void push(R (C::*func_ptr)(T1, T2, T3, T4, T5, T6) ) const
	{
		struct wrapper_creator
		{
			typedef R (C::*memfunptr)(T1, T2, T3, T4, T5, T6) ;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);
				T3 arg3;
				st.get(arg3, 4);
				T4 arg4;
				st.get(arg4, 5);
				T5 arg5;
				st.get(arg5, 6);
				T6 arg6;
				st.get(arg6, 7);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2, arg3, arg4, arg5, arg6));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	void push(R (C::*func_ptr)(T1, T2, T3, T4, T5, T6) const) const
	{
		struct wrapper_creator
		{
			typedef R (C::*memfunptr)(T1, T2, T3, T4, T5, T6) const;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);
				T3 arg3;
				st.get(arg3, 4);
				T4 arg4;
				st.get(arg4, 5);
				T5 arg5;
				st.get(arg5, 6);
				T6 arg6;
				st.get(arg6, 7);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2, arg3, arg4, arg5, arg6));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	void push(void (C::*func_ptr)(T1, T2, T3, T4, T5, T6) ) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2, T3, T4, T5, T6) ;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);
				T3 arg3;
				st.get(arg3, 4);
				T4 arg4;
				st.get(arg4, 5);
				T5 arg5;
				st.get(arg5, 6);
				T6 arg6;
				st.get(arg6, 7);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2, arg3, arg4, arg5, arg6));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	void push(void (C::*func_ptr)(T1, T2, T3, T4, T5, T6) const) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2, T3, T4, T5, T6) const;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);
				T3 arg3;
				st.get(arg3, 4);
				T4 arg4;
				st.get(arg4, 5);
				T5 arg5;
				st.get(arg5, 6);
				T6 arg6;
				st.get(arg6, 7);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2, arg3, arg4, arg5, arg6));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	void push(R (C::*func_ptr)(T1, T2, T3, T4, T5, T6, T7) ) const
	{
		struct wrapper_creator
		{
			typedef R (C::*memfunptr)(T1, T2, T3, T4, T5, T6, T7) ;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);
				T3 arg3;
				st.get(arg3, 4);
				T4 arg4;
				st.get(arg4, 5);
				T5 arg5;
				st.get(arg5, 6);
				T6 arg6;
				st.get(arg6, 7);
				T7 arg7;
				st.get(arg7, 8);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	void push(R (C::*func_ptr)(T1, T2, T3, T4, T5, T6, T7) const) const
	{
		struct wrapper_creator
		{
			typedef R (C::*memfunptr)(T1, T2, T3, T4, T5, T6, T7) const;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);
				T3 arg3;
				st.get(arg3, 4);
				T4 arg4;
				st.get(arg4, 5);
				T5 arg5;
				st.get(arg5, 6);
				T6 arg6;
				st.get(arg6, 7);
				T7 arg7;
				st.get(arg7, 8);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	void push(void (C::*func_ptr)(T1, T2, T3, T4, T5, T6, T7) ) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2, T3, T4, T5, T6, T7) ;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);
				T3 arg3;
				st.get(arg3, 4);
				T4 arg4;
				st.get(arg4, 5);
				T5 arg5;
				st.get(arg5, 6);
				T6 arg6;
				st.get(arg6, 7);
				T7 arg7;
				st.get(arg7, 8);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	void push(void (C::*func_ptr)(T1, T2, T3, T4, T5, T6, T7) const) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2, T3, T4, T5, T6, T7) const;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);
				T3 arg3;
				st.get(arg3, 4);
				T4 arg4;
				st.get(arg4, 5);
				T5 arg5;
				st.get(arg5, 6);
				T6 arg6;
				st.get(arg6, 7);
				T7 arg7;
				st.get(arg7, 8);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	void push(R (C::*func_ptr)(T1, T2, T3, T4, T5, T6, T7, T8) ) const
	{
		struct wrapper_creator
		{
			typedef R (C::*memfunptr)(T1, T2, T3, T4, T5, T6, T7, T8) ;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);
				T3 arg3;
				st.get(arg3, 4);
				T4 arg4;
				st.get(arg4, 5);
				T5 arg5;
				st.get(arg5, 6);
				T6 arg6;
				st.get(arg6, 7);
				T7 arg7;
				st.get(arg7, 8);
				T8 arg8;
				st.get(arg8, 9);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	void push(R (C::*func_ptr)(T1, T2, T3, T4, T5, T6, T7, T8) const) const
	{
		struct wrapper_creator
		{
			typedef R (C::*memfunptr)(T1, T2, T3, T4, T5, T6, T7, T8) const;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);
				T3 arg3;
				st.get(arg3, 4);
				T4 arg4;
				st.get(arg4, 5);
				T5 arg5;
				st.get(arg5, 6);
				T6 arg6;
				st.get(arg6, 7);
				T7 arg7;
				st.get(arg7, 8);
				T8 arg8;
				st.get(arg8, 9);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	void push(void (C::*func_ptr)(T1, T2, T3, T4, T5, T6, T7, T8) ) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2, T3, T4, T5, T6, T7, T8) ;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);
				T3 arg3;
				st.get(arg3, 4);
				T4 arg4;
				st.get(arg4, 5);
				T5 arg5;
				st.get(arg5, 6);
				T6 arg6;
				st.get(arg6, 7);
				T7 arg7;
				st.get(arg7, 8);
				T8 arg8;
				st.get(arg8, 9);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename R, typename C, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	void push(void (C::*func_ptr)(T1, T2, T3, T4, T5, T6, T7, T8) const) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2, T3, T4, T5, T6, T7, T8) const;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
				T1 arg1;
				st.get(arg1, 2);
				T2 arg2;
				st.get(arg2, 3);
				T3 arg3;
				st.get(arg3, 4);
				T4 arg4;
				st.get(arg4, 5);
				T5 arg5;
				st.get(arg5, 6);
				T6 arg6;
				st.get(arg6, 7);
				T7 arg7;
				st.get(arg7, 8);
				T8 arg8;
				st.get(arg8, 9);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}
// END MEM FUNC GLUE

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

	void repush(int index) const;

	void pop(int num) const;

	int size() const;
	bool is_valid_index(int index) const;
	int normalize_index(int index) const;
	int upvalue_index(int index) const;

	type element_type(int index) const;

	reference create_userdata(std::size_t size) const;
	reference create_table() const;

	void get_table_field(int t) const;
	void set_table_field(int t) const;

	void raw_get_table_field(int t) const;
	void raw_set_table_field(int t) const;

	void call(int nargs, int nresults) const;
private:
	lua_State* L;

	template<typename Memfun>
	void push_memfun_wrapper(bindable_funcptr wrapper, Memfun memfun) const
	{
		void* ptr = NULL;
		reference ud = create_userdata(sizeof(memfun));
		ptr = ud;
		*(static_cast<Memfun*>(ptr)) = memfun;

		push(ud);
		push(wrapper, 1);
	}
};

}

#endif
