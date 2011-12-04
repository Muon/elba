#ifndef ELBA_ERROR_HPP
#define ELBA_ERROR_HPP

#include "elba_activation_record.hpp"

#include <stdexcept>
#include <vector>

struct lua_State;

namespace elba
{

class error
{
public:
	error(lua_State* L) : L(L) {}
	virtual ~error() throw() {}

	lua_State* state() const { return L; }
private:
	lua_State* L;
};

class out_of_memory_error : public std::exception, public error
{
public:
	out_of_memory_error(lua_State* L, const char* msg = "not enough memory") : error(L), message(msg) {}
	const char* what() const throw() { return message; }
private:
	const char* message;
};

class syntax_error : public std::logic_error, public error
{
public:
	syntax_error(lua_State* L, const std::string& msg) : std::logic_error(msg), error(L) {}
};

class error_handler_error : public std::runtime_error, public error
{
public:
	error_handler_error(lua_State* L, const std::string& msg) : std::runtime_error(msg), error(L) {}
};

class runtime_error : public std::runtime_error, public error
{
public:
	typedef std::vector<activation_record> call_stack_type;

	runtime_error(lua_State* L, const std::string& msg);
	virtual ~runtime_error() throw() {}

	const call_stack_type& call_stack() const { return call_stack_; }
private:
	call_stack_type call_stack_;
};

class conversion_error : public std::runtime_error, public error
{
public:
	conversion_error(lua_State* L, const std::string& msg = "type conversion failed") : runtime_error(msg), error(L) {}
};

}

#endif // ELBA_ERROR_HPP
