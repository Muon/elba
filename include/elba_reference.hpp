#ifndef ELBA_REFERENCE_HPP
#define ELBA_REFERENCE_HPP

#include "elba_stack.hpp"

struct lua_State;

namespace elba
{

class reference
{
public:
	reference& operator=(const reference& other);
	
	template<typename T>
	reference& set_ref(const T& val)
	{
		stack st(L);
		st.push(val);
		set_ref();
		
		return *this;
	}

protected:
	reference();
	reference(lua_State* L);
	reference(const reference& other);
	~reference();

	void set_ref();
	void push_ref() const;

	lua_State* L;
private:
	int ref;

	friend class stack;
};

}

#endif // ELBA_REFERENCE_HPP
