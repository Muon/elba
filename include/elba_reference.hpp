#ifndef ELBA_REFERENCE_HPP
#define ELBA_REFERENCE_HPP

#include <elba_stack.hpp>

namespace elba
{

class reference
{
protected:
	reference(lua_State* L);
	reference(lua_State* L, int index);
	
	~reference();
	
	int get_index() const { return stack_index; }
private:
	stack cur_stack;
	int stack_index;
};

}

#endif // ELBA_REFERENCE_HPP
