#ifndef ELBA_CLASSBINDER_HPP
#define ELBA_CLASSBINDER_HPP

#include "elba_reference.hpp"

#include <boost/type_traits/is_member_function_pointer.hpp>
#include <boost/utility/enable_if.hpp>

namespace elba
{

template<typename T>
class class_binder
{
public:
	class_binder(lua_State* L, const char* name)
		: L(L)
		, metatable(make_table(L))
		, methods(make_table(L))
		, types(make_table(L))
		, convops(make_table(L))
		, statics(make_table(L))
	{
		stack st(L);
		st.push(metatable);

		st.set_table_field(-1, "__index", methods);
		st.set_table_field(-1, "__gc", finalize);

		st.set_table_field(-1, "convops", convops);
		st.set_table_field(-1, "name", name);

		st.set_table_field(stack::registry_index, class_id<T>(), metatable);

		st.set_table_field(stack::globals_index, name, statics);

		st.pop(1);
	}

	class_binder& constructor()
	{
		struct initializer
		{
			static int initialize(lua_State* L)
			{
				stack st(L);
				new(st.create_userdata(sizeof(T))) T();

				st.repush(st.upvalue_index(1));
				st.set_metatable(-2);

				return 1;
			}
		};

		stack st(L);

		st.push(statics);

		st.push("new");

		st.push(metatable);
		st.push(initializer::initialize, 1);

		st.set_table_field(-3);

		st.pop(1);

		return *this;
	}

	template<typename U, typename V>
	typename boost::enable_if<boost::is_member_function_pointer<V>,
		class_binder<T>&>::type set(U name, V val)
	{
		return method(name, val);
	}

	template<typename U, typename V>
	typename boost::disable_if<boost::is_member_function_pointer<V>,
		class_binder<T>&>::type set(U name, V val)
	{
		return static_(name, val);
	}

	template<typename U, typename V>
	class_binder<T>& method(U name, V val)
	{
		methods.set(name, val);
		return *this;
	}

	template<typename U, typename V>
	class_binder<T>& static_(U name, V val)
	{
		statics.set(name, val);
		return *this;
	}

	template<typename U>
	class_binder<T>& conversion_operator()
	{
		struct implicit_conversion
		{
			static U convert(const T& value) { return static_cast<U>(value); }
		};

		convops.set(class_id<U>(), implicit_conversion::convert);
		return *this;
	}

private:
	static void finalize(T* object) { object->~T(); }

	lua_State* const L;

	reference metatable;
	reference methods;
	reference types;
	reference convops;
	reference statics;
};

}

#endif // ELBA_CLASSBINDER_HPP
