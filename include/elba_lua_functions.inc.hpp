#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

#define ARGUMENT(z, n, data) const T ## n & arg ## n
#define ARGUMENT_LIST(n) BOOST_PP_ENUM(n, ARGUMENT, BOOST_PP_EMPTY)

#define PUSH_ARGUMENT(z, n, data) st.push(arg ## n);
#define PUSH_ARGUMENTS(nargs) BOOST_PP_REPEAT(nargs, PUSH_ARGUMENT, BOOST_PP_EMPTY)

#define REFERENCE_CALL(z, nargs, data) \
	BOOST_PP_EXPR_IF(nargs, template<) \
		BOOST_PP_ENUM_PARAMS(nargs, typename T) \
	BOOST_PP_EXPR_IF(nargs, >) \
	reference operator()(ARGUMENT_LIST(nargs)) \
	{ \
		stack st(L); \
		st.push(*this); \
		PUSH_ARGUMENTS(nargs) \
		st.call(nargs, 1); \
		reference ref(L, -1); \
		st.pop(1); \
		return ref; \
	}

#ifndef ELBA_LUA_FUNCTION_CALL_ARITY
#define ELBA_LUA_FUNCTION_CALL_ARITY 8
#endif

BOOST_PP_REPEAT(ELBA_LUA_FUNCTION_CALL_ARITY, REFERENCE_CALL, BOOST_PP_EMPTY)

#undef REFERENCE_CALL
#undef PUSH_ARGUMENTS
#undef PUSH_ARGUMENT
#undef ARGUMENT_LIST
#undef ARGUMENT
