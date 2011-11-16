#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/logical/not.hpp>
#include <boost/preprocessor/logical/and.hpp>
#include <boost/preprocessor/logical/or.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

#define PTR_SIGNATURE(name, nargs, voidness) \
	BOOST_PP_IF(voidness, void, R) (*name)(BOOST_PP_ENUM_PARAMS(nargs, T))

#define FREE_FUNCTION_DECL(nargs, voidness) \
	BOOST_PP_EXPR_IF(BOOST_PP_OR(BOOST_PP_NOT(voidness), nargs), template<)\
		BOOST_PP_ENUM_PARAMS(nargs, typename T) BOOST_PP_COMMA_IF(BOOST_PP_AND(BOOST_PP_NOT(voidness), nargs)) \
		BOOST_PP_EXPR_IF(BOOST_PP_NOT(voidness), typename R) \
	BOOST_PP_EXPR_IF(BOOST_PP_OR(BOOST_PP_NOT(voidness), nargs), >) \
	void push(PTR_SIGNATURE(funcptr, nargs, voidness)) const;

#define FREE_FUNCTION_DECLS(z, n, data) \
	FREE_FUNCTION_DECL(n, 0) \
	FREE_FUNCTION_DECL(n, 1)

#ifndef ELBA_FREE_FUNCTION_PUSHER_ARITY
#define ELBA_FREE_FUNCTION_PUSHER_ARITY 8
#endif

BOOST_PP_REPEAT(ELBA_FREE_FUNCTION_PUSHER_ARITY, FREE_FUNCTION_DECLS, BOOST_PP_EMPTY)

#undef FREE_FUNCTION_DECLS
#undef FREE_FUNCTION_DECL
#undef PTR_SIGNATURE
