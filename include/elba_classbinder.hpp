#ifndef ELBA_CLASSBINDER_HPP
#define ELBA_CLASSBINDER_HPP

#include "elba_reference.hpp"

namespace elba
{

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

				void* mem = NULL;
				reference ud = st.create_userdata(sizeof(T));
				mem = ud;

				new(mem) T();

				ud.metatable(reference(L, st.upvalue_index(1)));

				return 1;
			}
		};

		stack st(L);
		st.push(metatable);
		st.push(initializer::initialize, 1);

		reference init_ref(L, stack::top);

		st.pop(1);

		methods["new"] = init_ref;

		return *this;
	}

	template<typename T, typename U>
	class_binder& set(T name, U val)
	{
		methods[name] = val;
		return *this;
	}

	template<typename T, typename U>
	class_binder& method(T name, U func)
	{
		methods[name] = func;
		return *this;
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
		metatable["__gc"] = finalizer;
	}

	reference metatable;
	reference methods;
private:
	lua_State* L;

};

}

#endif // ELBA_CLASSBINDER_HPP
