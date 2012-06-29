#ifndef BOOST_PP_IS_ITERATING

#ifndef ELBA_LUA_FUNCTIONS_INC_HPP
#define ELBA_LUA_FUNCTIONS_INC_HPP

#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>

#ifndef ELBA_LUA_FUNCTION_CALL_ARITY
#define ELBA_LUA_FUNCTION_CALL_ARITY 8
#endif

#define BOOST_PP_ITERATION_LIMITS (0, ELBA_LUA_FUNCTION_CALL_ARITY)
#define BOOST_PP_FILENAME_1 "elba_lua_functions.inc.hpp"
#include BOOST_PP_ITERATE()

#endif

#else

#define nargs BOOST_PP_ITERATION()
#define ARGUMENT(z, n, data) const T ## n & arg ## n
#define PUSH_ARGUMENT(z, n, data) st.push(arg ## n);

BOOST_PP_EXPR_IF(nargs, template<)
	BOOST_PP_ENUM_PARAMS(nargs, typename T)
BOOST_PP_EXPR_IF(nargs, >)
reference operator()(BOOST_PP_ENUM(nargs, ARGUMENT, BOOST_PP_EMPTY))
{
	stack st(L);
	st.push(*this);
	BOOST_PP_REPEAT(nargs, PUSH_ARGUMENT, BOOST_PP_EMPTY)
	st.call(nargs, 1);
	reference ref(L, -1);
	st.pop(1);
	return ref;
}

#undef ARGUMENT
#undef PUSH_ARGUMENT

#endif
