#ifndef ELBA_CLASSBINDER_HPP
#define ELBA_CLASSBINDER_HPP

#include "elba_reference.hpp"

namespace elba
{

class class_binder;

template<> void stack::push<class_binder>(const class_binder& binder) const;

class class_binder
{
public:
	class_binder(lua_State* L);

	template<typename T>
	class_binder& constructor()
	{
		struct initializer
		{
			static int initialize(lua_State* L)
			{
				stack st(L);

				reference ud = st.create_userdata(sizeof(T));

				new(static_cast<void*>(ud)) T();

				ud.metatable(reference(L, st.upvalue_index(1)));

				return 1;
			}
		};

		stack st(L);
		st.push(metatable);
		st.push(initializer::initialize, 1);

		methods.set("new", reference(L, stack::top));;

		st.pop(1);

		return *this;
	}

	template<typename T, typename U>
	class_binder& set(T name, U val)
	{
		methods.set(name, val);
		return *this;
	}

	template<typename T, typename U>
	class_binder& method(T name, U func)
	{
		return set(name, func);
	}

	template<typename T>
	class_binder& destructor()
	{
		struct finalizer
		{
			static void finalize(T* object)
			{
				object->~T();
			}
		};

		destructor(finalizer::finalize);

		return *this;
	}

	template<typename T>
	void destructor(void (*finalizer)(T*))
	{
		metatable.set("__gc", finalizer);
	}

	reference metatable;
	reference methods;
private:
	lua_State* const L;
	// FIXME: Why can't I make this more specific on GCC?
	template<typename T> friend void stack::push(const T& binder) const;
};

}

#endif // ELBA_CLASSBINDER_HPP
