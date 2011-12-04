#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/logical/not.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

#define GET_ARGUMENT(z, n, data) st.get<T ## n>(BOOST_PP_ADD(n, 2))
#define GET_ARGUMENTS(n) BOOST_PP_ENUM(n, GET_ARGUMENT, BOOST_PP_EMPTY)

#define PTR_SIGNATURE(name, nargs, voidness, constness) \
	BOOST_PP_IF(voidness, void, R) (C::*name)(BOOST_PP_ENUM_PARAMS(nargs, T)) BOOST_PP_EXPR_IF(constness, const)

#define MEMBER_FUNCTION(nargs, voidness, constness) \
	template<\
		BOOST_PP_ENUM_PARAMS(nargs, typename T) BOOST_PP_COMMA_IF(nargs) \
		BOOST_PP_EXPR_IF(BOOST_PP_NOT(voidness), typename R) BOOST_PP_COMMA_IF(BOOST_PP_NOT(voidness)) \
		typename C \
	> \
	void push(PTR_SIGNATURE(funcptr, nargs, voidness, constness)) const \
	{ \
		typedef PTR_SIGNATURE(funcptr_type, nargs, voidness, constness); \
		\
		struct wrapper_creator \
		{ \
			static int wrapper(lua_State* L) \
			{ \
				stack st(L); \
				bool exception_thrown = false; \
				\
				try \
				{ \
					funcptr_type func = *(static_cast<funcptr_type*>(st.get<void*>(st.upvalue_index(1)))); \
					C* object = st.get<C*>(1); \
					BOOST_PP_EXPR_IF(BOOST_PP_NOT(voidness), st.push)((object->*func)( \
						GET_ARGUMENTS(nargs) \
					)); \
				} \
				catch(...) \
				{ \
					exception_thrown = true; \
					st.handle_active_exception(); \
				} \
				if(exception_thrown) \
					st.raise_error(); \
				return BOOST_PP_IF(voidness, 0, 1); \
			} \
		}; \
		push_memfun_wrapper(wrapper_creator::wrapper, funcptr); \
	}

#define MEMBER_FUNCTIONS(z, n, data) \
	MEMBER_FUNCTION(n, 0, 0) \
	MEMBER_FUNCTION(n, 1, 0) \
	MEMBER_FUNCTION(n, 0, 1) \
	MEMBER_FUNCTION(n, 1, 1)

#ifndef ELBA_MEMBER_FUNCTION_PUSHER_ARITY
#define ELBA_MEMBER_FUNCTION_PUSHER_ARITY 8
#endif

BOOST_PP_REPEAT(ELBA_MEMBER_FUNCTION_PUSHER_ARITY, MEMBER_FUNCTIONS, BOOST_PP_EMPTY)

#undef MEMBER_FUNCTIONS
#undef MEMBER_FUNCTION
#undef PTR_SIGNATURE
#undef GET_ARGUMENTS
#undef GET_ARGUMENT
