#ifndef ELBA_VALUEREF_HPP
#define ELBA_VALUEREF_HPP

#include "elba_reference.hpp"
#include "elba_stack.hpp"

namespace elba
{

class value_ref : public reference
{
public:
	value_ref(lua_State* L, int index);

	template<typename T>
	T get()
	{
		stack st(L);

		push_ref();

		T tmp;
		st.get(tmp, stack::top);

		st.pop(1);

		return tmp;
	}

	template<typename T> operator T() { return get<T>(); }
};

template<> value_ref::operator const char*();

}

#endif // ELBA_VALUEREF_HPP
