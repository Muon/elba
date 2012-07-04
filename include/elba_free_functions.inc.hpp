#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/logical/not.hpp>
#include <boost/preprocessor/logical/and.hpp>
#include <boost/preprocessor/logical/or.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

#define GET_ARGUMENT(z, n, data) st.get<T ## n>(BOOST_PP_ADD(n, 1))
#define GET_ARGUMENTS(n) BOOST_PP_ENUM(n, GET_ARGUMENT, BOOST_PP_EMPTY)

#define PTR_SIGNATURE(name, nargs, voidness) \
	BOOST_PP_IF(voidness, void, R) (*name)(BOOST_PP_ENUM_PARAMS(nargs, T))

#define FREE_FUNCTION(nargs, voidness) \
	BOOST_PP_EXPR_IF(BOOST_PP_OR(BOOST_PP_NOT(voidness), nargs), template<)\
		BOOST_PP_ENUM_PARAMS(nargs, typename T) BOOST_PP_COMMA_IF(BOOST_PP_AND(BOOST_PP_NOT(voidness), nargs)) \
		BOOST_PP_EXPR_IF(BOOST_PP_NOT(voidness), typename R) \
	BOOST_PP_IF(BOOST_PP_OR(BOOST_PP_NOT(voidness), nargs), >, inline) \
	void stack::push(PTR_SIGNATURE(funcptr, nargs, voidness)) const \
	{ \
		typedef PTR_SIGNATURE(funcptr_type, nargs, voidness); \
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
					funcptr_type func = reinterpret_cast<funcptr_type>(st.get<bindable_funcptr>(st.upvalue_index(1))); \
					BOOST_PP_EXPR_IF(BOOST_PP_NOT(voidness), st.push)(func(GET_ARGUMENTS(nargs))); \
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
		push(reinterpret_cast<bindable_funcptr>(funcptr)); \
		push(wrapper_creator::wrapper, 1); \
	}

#define FREE_FUNCTIONS(z, n, data) \
	FREE_FUNCTION(n, 0) \
	FREE_FUNCTION(n, 1)

#ifndef ELBA_FREE_FUNCTION_PUSHER_ARITY
#error ELBA_FREE_FUNCTION_PUSHER_ARITY must be defined.
#endif

BOOST_PP_REPEAT(ELBA_FREE_FUNCTION_PUSHER_ARITY, FREE_FUNCTIONS, BOOST_PP_EMPTY)

#undef FREE_FUNCTIONS
#undef FREE_FUNCTION
#undef PTR_SIGNATURE
#undef GET_ARGUMENTS
#undef GET_ARGUMENT
