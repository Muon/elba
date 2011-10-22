#ifndef ELBA_ACTIVATION_RECORD_HPP
#define ELBA_ACTIVATION_RECORD_HPP

#include <string>
#include <utility>
#include <iosfwd>

struct lua_State;
struct lua_Debug;

namespace elba
{

class activation_record
{
public:
	activation_record(lua_State* L, lua_Debug& dbg);

	int line() const { return line_; }
	const std::pair<int, int>& source_range() const { return source_range_; }

	const char* relevant_source() const { return relevant_source_; }

	const char* name() const { return name_.c_str(); }
	const char* name_what() const { return name_what_; }

	const char* function_call_kind() const { return function_call_kind_; }
private:
	int line_;
	std::pair<int, int> source_range_;

	char relevant_source_[60];

	std::string name_;

	const char* name_what_;
	const char* function_call_kind_;
};

std::ostream& operator<<(std::ostream& os, const activation_record& ar);

}

#endif // ELBA_ACTIVATION_RECORD_HPP
