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

	operator lua_State* () const;

	void open_libs() const;
	void do_string(const char* string) const;
private:

	// ensure that the state is destroyed after the table
	struct state_destruction_deferral
	{
		state_destruction_deferral();
		~state_destruction_deferral();

		lua_State* L;
	} state_container;

public: // avoiding init order complaints with -pedantic
	table globals;
};

}

#endif
