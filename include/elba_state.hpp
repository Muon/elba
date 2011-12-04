#ifndef ELBA_STATE_HPP
#define ELBA_STATE_HPP

#include "elba_reference.hpp"
#include "elba_classbinder.hpp"

struct lua_State;

namespace elba
{

class state
{
public:
	state();

	operator lua_State* () const;

	void open_libs() const;
	void do_string(const std::string& string) const;

	reference create_table() const;
private:
	state(const state& other);
	state& operator=(const state& other);

	// ensure that the state is destroyed after the table
	struct state_destruction_deferral
	{
		state_destruction_deferral();
		~state_destruction_deferral();

		lua_State* const L;
	} state_container;

public: // avoiding init order complaints with -pedantic
	reference globals;
};

}

#endif
