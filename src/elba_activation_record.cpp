#include <elba_activation_record.hpp>

extern "C"
{
#include <lua.h>
}

#include <cstring>
#include <ostream>

namespace elba
{

activation_record::activation_record(lua_State* L, lua_Debug& dbg)
{
	lua_getinfo(L, "nSl", &dbg);

	line_ = dbg.currentline;
	source_range_.first = dbg.linedefined;
	source_range_.second = dbg.lastlinedefined;

	std::strncpy(relevant_source_, dbg.short_src, sizeof relevant_source_);

	if(dbg.name)
		name_ = dbg.name;

	name_what_ = dbg.namewhat;
	function_call_kind_ = dbg.what;
}

std::ostream& operator<<(std::ostream& os, const activation_record& ar)
{
	os << ar.relevant_source() << ":";

	if(ar.line() > 0)
		os << ar.line() << ':';

	if(ar.name_what()[0] != '\0')
		os << " in function '" << ar.name() << '\'';
	else
	{
		if(ar.function_call_kind()[0] == 'm')
			os << " in main chunk";
		else if(ar.function_call_kind()[0] == 'C' || ar.function_call_kind()[0] == 't')
			os << " ?";
		else
			os << " in function <" << ar.relevant_source() << ':' << ar.line() << '>';
	}

	return os;
}

}
