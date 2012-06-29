#ifndef BOOST_PP_IS_ITERATING

#ifndef ELBA_LUA_FUNCTIONS_INDEX_INC_HPP
#define ELBA_LUA_FUNCTIONS_INDEX_INC_HPP

#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>

#ifndef ELBA_LUA_FUNCTION_CALL_ARITY
#error ELBA_LUA_FUNCTION_CALL_ARITY must be defined.
#endif

#define BOOST_PP_ITERATION_LIMITS (0, ELBA_LUA_FUNCTION_CALL_ARITY)
#define BOOST_PP_FILENAME_1 "elba_lua_functions_index.inc.hpp"
#include BOOST_PP_ITERATE()

#endif

#else

#define nargs BOOST_PP_ITERATION()

#define ARGUMENT(z, n, text) const T ## n & text ## n

BOOST_PP_EXPR_IF(nargs, template<)
	BOOST_PP_ENUM_PARAMS(nargs, typename T)
BOOST_PP_EXPR_IF(nargs, >)
reference operator()(BOOST_PP_ENUM(nargs, ARGUMENT, arg))
{
	return owner_table.get<reference>(ref)(BOOST_PP_ENUM_PARAMS(nargs, arg));
}

#endif
