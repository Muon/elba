#ifndef ELBA_STATE_HPP
#define ELBA_STATE_HPP

#include "elba_table.hpp"

struct lua_State;

namespace elba
{

class state
{
public:
	state();
	~state();
	
	operator lua_State* () const;

	void open_libs() const;
	void do_string(const char* string) const;
private:
	lua_State* L;

public: // avoiding init order complaints with -pedantic
	table globals;
};

}

#endif
