#ifndef ELBA_TEST_UTILITIES_HPP
#define ELBA_TEST_UTILITIES_HPP

#include <cassert>
#include <cstring>
#include <stdexcept>
#include <typeinfo>
#include <cstdlib>

#ifdef __GNUC__
#include <cxxabi.h>

inline std::string demangle(const char* name)
{
	int status;

	char* tmp = abi::__cxa_demangle(name, NULL, NULL, &status);
	assert(status == 0);

	std::string demangled(tmp);

	std::free(tmp);

	return demangled;
}

#else

inline std::string demangle(const char* name) { return name; }

#endif

template <typename T> std::string type_name()
{
	return demangle(typeid(T).name());
}

template <typename T> std::string type_name(const T& t)
{
	static_cast<void>(t);
	return type_name<T>();
}

class test_failure : public std::logic_error
{
public:
	explicit test_failure(const std::string& msg) : logic_error(msg) {}
};

class exception_failure : public test_failure
{
public:
	explicit exception_failure(const std::string& msg) : test_failure(msg) {}
};

class equality_failure : public test_failure
{
public:
	explicit equality_failure(const std::string& msg) : test_failure(msg) {}
};

template<typename T> void handle_expected_exception(const char* what, const char* location)
{
	try
	{
		throw;
	}
	catch(const T& e)
	{
		if(std::strcmp(what, e.what()) != 0)
		{
			std::string message = location;
			message += ": expected error message of \"";
			message += what;
			message += "\", got \"";
			message += e.what();
			message += '"';

			throw exception_failure(message);
		}
	}
	catch(const exception_failure& e)
	{
		throw;
	}
	catch(const std::exception& e)
	{
		std::string message = location;
		message += ": expected exception of type ";
		message += type_name<T>();
		message += ", caught ";
		message += type_name(e);
		message += ": ";
		message += e.what();

		throw exception_failure(message);
	}
	catch(...)
	{
		std::string message = location;
		message += ": caught exception of unknown type, expected ";
		message += type_name<T>();

		throw exception_failure(message);
	}
}

inline void handle_unexpected_exception(const char* location)
{
	try
	{
		throw;
	}
	catch(const std::exception& e)
	{
		std::string message = location;
		message += ": unexpected exception of type ";
		message += type_name(e);

		throw exception_failure(message);
	}
	catch(...)
	{
		std::string message = location;
		message += ": unexpected exception of unknown type caught";

		throw exception_failure(message);
	}
}

#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)
#define DELAY_MACRO(x) x

#define LOCATION DELAY_MACRO(__FILE__ ":" STRINGIFY(__LINE__))

#define check_exception_thrown(expr, except, str) { try { expr; throw exception_failure(LOCATION ": failed to throw an exception, expected " #except); } catch(...) { handle_expected_exception<except>(str, LOCATION); } }
#define check_no_exception_thrown(expr) { try { expr; } catch(...) { handle_unexpected_exception(LOCATION); } }

#define assert_true(a) assert_equal(!!(a), true)
#define assert_false(b) assert_equal(!!(b), false)

#define assert_equal(a, b) \
{ \
	if(!((a) == (b))) \
		throw equality_failure(LOCATION ": comparison !(" #a " == " #b ") failed"); \
	if(!((b) == (a))) \
		throw equality_failure(LOCATION ": comparison !(" #b " == " #a ") failed"); \
	if((a) != (b)) \
		throw equality_failure(LOCATION ": comparison " #a " != " #b " failed"); \
	if((b) != (a)) \
		throw equality_failure(LOCATION ": comparison " #b " != " #a " failed"); \
}

#define assert_not_equal(a, b) \
{ \
	if(!((a) != (b))) \
		throw equality_failure(LOCATION ": comparison !(" #a " != " #b ") failed"); \
	if(!((b) != (a))) \
		throw equality_failure(LOCATION ": comparison !(" #b " != " #a ") failed"); \
	if((a) == (b)) \
		throw equality_failure(LOCATION ": comparison " #a " == " #b " failed"); \
	if((b) == (a)) \
		throw equality_failure(LOCATION ": comparison " #b " == " #a " failed"); \
}

#endif // ELBA_TEST_UTILITIES_HPP
