#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

#define ARGUMENT(z, n, text) const T ## n & text ## n
#define ARGUMENT_LIST(n) BOOST_PP_ENUM(n, ARGUMENT, arg)

#define PUSH_ARGUMENT(z, n, text) st.push(text ## n);
#define PUSH_ARGUMENTS(nargs) BOOST_PP_REPEAT(nargs, PUSH_ARGUMENT, arg)

#define REFERENCE_FORWARD_CALL(z, nargs, data) \
	BOOST_PP_EXPR_IF(nargs, template<) \
		BOOST_PP_ENUM_PARAMS(nargs, typename T) \
	BOOST_PP_EXPR_IF(nargs, >) \
	reference operator()(ARGUMENT_LIST(nargs)) \
	{ \
		return owner_table.get<reference>(ref)(BOOST_PP_ENUM_PARAMS(nargs, arg)); \
	}

#ifndef ELBA_LUA_FUNCTION_CALL_ARITY
#error ELBA_LUA_FUNCTION_CALL_ARITY must be defined.
#endif

BOOST_PP_REPEAT(ELBA_LUA_FUNCTION_CALL_ARITY, REFERENCE_FORWARD_CALL, BOOST_PP_EMPTY)

#undef REFERENCE_FORWARD_CALL
#undef PUSH_ARGUMENTS
#undef PUSH_ARGUMENT
#undef ARGUMENT_LIST
#undef ARGUMENT
