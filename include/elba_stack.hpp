#ifndef ELBA_STACK_HPP
#define ELBA_STACK_HPP

#include <string>

struct lua_State;

namespace elba
{

struct nil_type;

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

	stack(lua_State* L) : L(L) {}

	void push(char c) const;

	void push(signed char integer) const;
	void push(short integer) const;
	void push(int integer) const;
	void push(long integer) const;

	void push(unsigned char c) const;
	void push(unsigned short integer) const;
	void push(unsigned int integer) const;
	void push(unsigned long integer) const;

	void push(double number) const;
	void push(float number) const;

	void push(const std::string& string) const;
	void push(const char* string) const;

	void push(const bindable_funcptr ptr, int num_upvalues = 0) const;

	void push(bool boolean) const;

	void push(const nil_type& n) const;

	void push(void* data) const;

	template<typename T>
	void push(const T& val) const
	{
		void* ud = create_userdata(sizeof(T));

		new(ud) T(val);
	}

	template<typename T>
	void push(T* ptr) const
	{
		push(static_cast<void*>(ptr));
	}

	template<typename T>
	T get(int idx = stack::top) const
	{
		return get_resolver<T>::get(*this, idx);
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

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

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

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

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

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				(object->*func)();

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

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				(object->*func)();

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

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(st.get<T1>(2)));

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

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(st.get<T1>(2)));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename C, typename T1>
	void push(void (C::*func_ptr)(T1) ) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1) ;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				((object->*func)(st.get<T1>(2)));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename C, typename T1>
	void push(void (C::*func_ptr)(T1) const) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1) const;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				((object->*func)(st.get<T1>(2)));

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

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(st.get<T1>(2), st.get<T2>(3)));

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

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(st.get<T1>(2), st.get<T2>(3)));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename C, typename T1, typename T2>
	void push(void (C::*func_ptr)(T1, T2) ) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2) ;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				((object->*func)(st.get<T1>(2), st.get<T2>(3)));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename C, typename T1, typename T2>
	void push(void (C::*func_ptr)(T1, T2) const) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2) const;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				((object->*func)(st.get<T1>(2), st.get<T2>(3)));

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

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(st.get<T1>(2), st.get<T2>(3), st.get<T3>(4)));

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

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(st.get<T1>(2), st.get<T2>(3), st.get<T3>(4)));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename C, typename T1, typename T2, typename T3>
	void push(void (C::*func_ptr)(T1, T2, T3) ) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2, T3) ;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				((object->*func)(st.get<T1>(2), st.get<T2>(3), st.get<T3>(4)));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename C, typename T1, typename T2, typename T3>
	void push(void (C::*func_ptr)(T1, T2, T3) const) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2, T3) const;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				((object->*func)(st.get<T1>(2), st.get<T2>(3), st.get<T3>(4)));

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

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(st.get<T1>(2), st.get<T2>(3), st.get<T3>(4), st.get<T4>(5)));

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

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(st.get<T1>(2), st.get<T2>(3), st.get<T3>(4), st.get<T4>(5)));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename C, typename T1, typename T2, typename T3, typename T4>
	void push(void (C::*func_ptr)(T1, T2, T3, T4) ) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2, T3, T4) ;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				((object->*func)(st.get<T1>(2), st.get<T2>(3), st.get<T3>(4), st.get<T4>(5)));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename C, typename T1, typename T2, typename T3, typename T4>
	void push(void (C::*func_ptr)(T1, T2, T3, T4) const) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2, T3, T4) const;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				((object->*func)(st.get<T1>(2), st.get<T2>(3), st.get<T3>(4), st.get<T4>(5)));

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

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(st.get<T1>(2), st.get<T2>(3), st.get<T3>(4), st.get<T4>(5), st.get<T5>(6)));

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

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(st.get<T1>(2), st.get<T2>(3), st.get<T3>(4), st.get<T4>(5), st.get<T5>(6)));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename C, typename T1, typename T2, typename T3, typename T4, typename T5>
	void push(void (C::*func_ptr)(T1, T2, T3, T4, T5) ) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2, T3, T4, T5) ;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				((object->*func)(st.get<T1>(2), st.get<T2>(3), st.get<T3>(4), st.get<T4>(5), st.get<T5>(6)));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename C, typename T1, typename T2, typename T3, typename T4, typename T5>
	void push(void (C::*func_ptr)(T1, T2, T3, T4, T5) const) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2, T3, T4, T5) const;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				((object->*func)(st.get<T1>(2), st.get<T2>(3), st.get<T3>(4), st.get<T4>(5), st.get<T5>(6)));

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

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(st.get<T1>(2), st.get<T2>(3), st.get<T3>(4), st.get<T4>(5), st.get<T5>(6), st.get<T6>(7)));

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

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(st.get<T1>(2), st.get<T2>(3), st.get<T3>(4), st.get<T4>(5), st.get<T5>(6), st.get<T6>(7)));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename C, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	void push(void (C::*func_ptr)(T1, T2, T3, T4, T5, T6) ) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2, T3, T4, T5, T6) ;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				((object->*func)(st.get<T1>(2), st.get<T2>(3), st.get<T3>(4), st.get<T4>(5), st.get<T5>(6), st.get<T6>(7)));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename C, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	void push(void (C::*func_ptr)(T1, T2, T3, T4, T5, T6) const) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2, T3, T4, T5, T6) const;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				((object->*func)(st.get<T1>(2), st.get<T2>(3), st.get<T3>(4), st.get<T4>(5), st.get<T5>(6), st.get<T6>(7)));

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

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(st.get<T1>(2), st.get<T2>(3), st.get<T3>(4), st.get<T4>(5), st.get<T5>(6), st.get<T6>(7), st.get<T7>(8)));

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

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(st.get<T1>(2), st.get<T2>(3), st.get<T3>(4), st.get<T4>(5), st.get<T5>(6), st.get<T6>(7), st.get<T7>(8)));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename C, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	void push(void (C::*func_ptr)(T1, T2, T3, T4, T5, T6, T7) ) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2, T3, T4, T5, T6, T7) ;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				((object->*func)(st.get<T1>(2), st.get<T2>(3), st.get<T3>(4), st.get<T4>(5), st.get<T5>(6), st.get<T6>(7), st.get<T7>(8)));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename C, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	void push(void (C::*func_ptr)(T1, T2, T3, T4, T5, T6, T7) const) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2, T3, T4, T5, T6, T7) const;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				((object->*func)(st.get<T1>(2), st.get<T2>(3), st.get<T3>(4), st.get<T4>(5), st.get<T5>(6), st.get<T6>(7), st.get<T7>(8)));

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

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(st.get<T1>(2), st.get<T2>(3), st.get<T3>(4), st.get<T4>(5), st.get<T5>(6), st.get<T6>(7), st.get<T7>(8), st.get<T8>(9)));

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

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				st.push((object->*func)(st.get<T1>(2), st.get<T2>(3), st.get<T3>(4), st.get<T4>(5), st.get<T5>(6), st.get<T6>(7), st.get<T7>(8), st.get<T8>(9)));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename C, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	void push(void (C::*func_ptr)(T1, T2, T3, T4, T5, T6, T7, T8) ) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2, T3, T4, T5, T6, T7, T8) ;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				((object->*func)(st.get<T1>(2), st.get<T2>(3), st.get<T3>(4), st.get<T4>(5), st.get<T5>(6), st.get<T6>(7), st.get<T7>(8), st.get<T8>(9)));

				return 1;
			}
		};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}

	template<typename C, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	void push(void (C::*func_ptr)(T1, T2, T3, T4, T5, T6, T7, T8) const) const
	{
		struct wrapper_creator
		{
			typedef void (C::*memfunptr)(T1, T2, T3, T4, T5, T6, T7, T8) const;

			static int wrapper(lua_State* L)
			{
				stack st(L);

				memfunptr* func_ptrptr = st.get<memfunptr*>(st.upvalue_index(1));

				C* object = st.get<C*>(1);

				memfunptr func = *func_ptrptr;

				((object->*func)(st.get<T1>(2), st.get<T2>(3), st.get<T3>(4), st.get<T4>(5), st.get<T5>(6), st.get<T6>(7), st.get<T7>(8), st.get<T8>(9)));

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

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get<bindable_funcptr>(st.upvalue_index(1)));

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

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get<bindable_funcptr>(st.upvalue_index(1)));

				st.push(function_real(st.get<T1>(1)));

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

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get<bindable_funcptr>(st.upvalue_index(1)));

				function_real(st.get<T1>(1));

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

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get<bindable_funcptr>(st.upvalue_index(1)));

				st.push(function_real(st.get<T1>(1), st.get<T2>(2)));

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

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get<bindable_funcptr>(st.upvalue_index(1)));

				function_real(st.get<T1>(1), st.get<T2>(2));

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

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get<bindable_funcptr>(st.upvalue_index(1)));

				st.push(function_real(st.get<T1>(1), st.get<T2>(2), st.get<T3>(3)));

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

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get<bindable_funcptr>(st.upvalue_index(1)));

				function_real(st.get<T1>(1), st.get<T2>(2), st.get<T3>(3));

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

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get<bindable_funcptr>(st.upvalue_index(1)));

				st.push(function_real(st.get<T1>(1), st.get<T2>(2), st.get<T3>(3), st.get<T4>(4)));

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

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get<bindable_funcptr>(st.upvalue_index(1)));

				function_real(st.get<T1>(1), st.get<T2>(2), st.get<T3>(3), st.get<T4>(4));

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

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get<bindable_funcptr>(st.upvalue_index(1)));

				st.push(function_real(st.get<T1>(1), st.get<T2>(2), st.get<T3>(3), st.get<T4>(4), st.get<T5>(5)));

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

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get<bindable_funcptr>(st.upvalue_index(1)));

				function_real(st.get<T1>(1), st.get<T2>(2), st.get<T3>(3), st.get<T4>(4), st.get<T5>(5));

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

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get<bindable_funcptr>(st.upvalue_index(1)));

				st.push(function_real(st.get<T1>(1), st.get<T2>(2), st.get<T3>(3), st.get<T4>(4), st.get<T5>(5), st.get<T6>(6)));

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

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get<bindable_funcptr>(st.upvalue_index(1)));

				function_real(st.get<T1>(1), st.get<T2>(2), st.get<T3>(3), st.get<T4>(4), st.get<T5>(5), st.get<T6>(6));

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

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get<bindable_funcptr>(st.upvalue_index(1)));

				st.push(function_real(st.get<T1>(1), st.get<T2>(2), st.get<T3>(3), st.get<T4>(4), st.get<T5>(5), st.get<T6>(6), st.get<T7>(7)));

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

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get<bindable_funcptr>(st.upvalue_index(1)));

				function_real(st.get<T1>(1), st.get<T2>(2), st.get<T3>(3), st.get<T4>(4), st.get<T5>(5), st.get<T6>(6), st.get<T7>(7));

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

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get<bindable_funcptr>(st.upvalue_index(1)));

				st.push(function_real(st.get<T1>(1), st.get<T2>(2), st.get<T3>(3), st.get<T4>(4), st.get<T5>(5), st.get<T6>(6), st.get<T7>(7), st.get<T8>(8)));

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

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get<bindable_funcptr>(st.upvalue_index(1)));

				function_real(st.get<T1>(1), st.get<T2>(2), st.get<T3>(3), st.get<T4>(4), st.get<T5>(5), st.get<T6>(6), st.get<T7>(7), st.get<T8>(8));

				return 0;
			}
		};

		push(reinterpret_cast<bindable_funcptr>(func_ptr));
		push(wrapper_creator::wrapper, 1);
	}
// END FREE FUNC GLUE
	void repush(int index) const;

	void insert(int index) const;

	void remove(int index) const;

	void pop(int num) const;

	int size() const;
	bool is_valid_index(int index) const;
	int normalize_index(int index) const;
	int upvalue_index(int index) const;
	bool are_equal(int idx1, int idx2) const;

	type element_type(int index) const;

	void* create_userdata(std::size_t size) const;
	void create_table() const;
	void create_function(const std::string& buffer) const;
	void create_function(const std::string& buffer, const std::string& name) const;

	void get_table_field(int t) const;
	void set_table_field(int t) const;

	void raw_get_table_field(int t) const;
	void raw_set_table_field(int t) const;

	bool get_metatable(int t) const;
	void set_metatable(int t) const;

	void call(int nargs, int nresults) const;

	void raise_error() const;
private:
	lua_State* const L;

	template<typename Memfun>
	void push_memfun_wrapper(bindable_funcptr wrapper, Memfun memfun) const
	{
		push<Memfun>(memfun);
		push(wrapper, 1);
	}

	template<typename T>
	struct get_resolver
	{
		static T get(const stack& st, int idx)
		{
			T* p = st.get<T*>(idx);
			return *p;
		}
	};
};

template<typename T>
struct stack::get_resolver<T*>
{
	static T* get(const stack& st, int idx)
	{
		return static_cast<T*>(st.get<void*>(idx));
	}
};

template<typename T>
struct stack::get_resolver<T* const>
{
	static T* get(const stack& st, int idx)
	{
		return st.get<T*>(idx);
	}
};

template<typename T>
struct stack::get_resolver<T* volatile>
{
	static T* get(const stack& st, int idx)
	{
		return st.get<T*>(idx);
	}
};

template<typename T>
struct stack::get_resolver<T* const volatile>
{
	static T* get(const stack& st, int idx)
	{
		return st.get<T*>(idx);
	}
};

template<typename T>
struct stack::get_resolver<T&>
{
	static T& get(const stack& st, int idx)
	{
		return *(st.get<T*>(idx));
	}
};

template<> std::string stack::get<std::string>(int idx) const;
template<> const char* stack::get<const char*>(int idx) const;
template<> void* stack::get<void*>(int idx) const;
template<> char stack::get<char>(int idx) const;
template<> signed char stack::get<signed char>(int idx) const;
template<> int stack::get<int>(int idx) const;
template<> short stack::get<short>(int idx) const;
template<> long stack::get<long>(int idx) const;
template<> unsigned char stack::get<unsigned char>(int idx) const;
template<> unsigned short stack::get<unsigned short>(int idx) const;
template<> unsigned int stack::get<unsigned int>(int idx) const;
template<> unsigned long stack::get<unsigned long>(int idx) const;
template<> float stack::get<float>(int idx) const;
template<> bool stack::get<bool>(int idx) const;
template<> stack::bindable_funcptr stack::get<stack::bindable_funcptr>(int idx) const;

}

#endif
