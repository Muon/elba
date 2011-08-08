#ifndef ELBA_REFERENCE_HPP
#define ELBA_REFERENCE_HPP

#include "elba_stack.hpp"

struct lua_State;

namespace elba
{

class reference
{
public:
	reference();
	reference(lua_State* L);
	reference(lua_State* L, int index);
	reference(const reference& other);
	~reference();
	
	reference& operator=(const reference& other);
	
	template<typename T>
	reference& set_ref(const T& val)
	{
		stack st(L);
		st.push(val);
		set_ref();
		
		return *this;
	}
	
	template<typename T>
	T get() const
	{
		stack st(L);

		push_ref();

		T tmp;
		st.get(tmp, stack::top);

		st.pop(1);

		return tmp;
	}

	template<typename T> operator T() const { return get<T>(); }

protected:
	void set_ref();
	void push_ref() const;

	lua_State* L;
private:
	int ref;

	friend class stack;
};

template<> reference::operator const char*() const;

}

#endif // ELBA_REFERENCE_HPP
