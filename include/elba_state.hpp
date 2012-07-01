#ifndef ELBA_STATE_HPP
#define ELBA_STATE_HPP

#include "elba_reference.hpp"

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
	void do_string(const std::string& string) const;

	reference globals() const;

	reference create_table() const;
private:
	state(const state& other);
	state& operator=(const state& other);

	lua_State* L;
};

}

#endif
