#ifndef ELBA_STACK_HPP
#define ELBA_STACK_HPP

#include "elba_error.hpp"

#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include <string>

struct lua_State;

namespace elba
{

template<typename T> struct recursively_remove_pointer { typedef T type; };
template<typename T> struct recursively_remove_pointer<T*> { typedef typename recursively_remove_pointer<T>::type type; };
template<typename T> struct recursively_remove_pointer<T* const> { typedef typename recursively_remove_pointer<T>::type type; };
template<typename T> struct recursively_remove_pointer<T* volatile> { typedef typename recursively_remove_pointer<T>::type type; };
template<typename T> struct recursively_remove_pointer<T* const volatile> { typedef typename recursively_remove_pointer<T>::type type; };

template<typename T> struct clean_type { typedef typename boost::remove_cv<typename recursively_remove_pointer<typename boost::remove_reference<T>::type>::type>::type type; };

typedef void* class_id_type;
template<typename T> class_id_type class_id_() { static const char c = 0; return reinterpret_cast<class_id_type>(const_cast<char*>(&c)); }

template<typename T> class_id_type class_id() { return class_id_<typename clean_type<T>::type>(); }

struct nil_type;

class stack
{
public:
	typedef long integer_type;
	typedef int(*bindable_funcptr)(lua_State*);

	enum position
	{
		top = -1,
		bottom = 1
	};

	enum type
	{
		none = -1,
		nil,
		boolean,
		lightuserdata,
		number,
		string,
		table,
		function,
		userdata,
		thread
	};

	stack(lua_State* L) : L(L) {}

	void push(char c) const;

	void push(signed char integer) const { push(static_cast<integer_type>(integer)); }
	void push(short integer) const { push(static_cast<integer_type>(integer)); }
	void push(int integer) const { push(static_cast<integer_type>(integer)); }
	void push(long integer) const;

	void push(unsigned char c) const { push(static_cast<integer_type>(c)); }
	void push(unsigned short integer) const { push(static_cast<integer_type>(integer)); }
	void push(unsigned int integer) const { push(static_cast<integer_type>(integer)); }
	void push(unsigned long integer) const { push(static_cast<integer_type>(integer)); }

	void push(double number) const;
	void push(float number) const;

	void push(const std::string& string) const;
	void push(const char* string) const;

	void push(const bindable_funcptr ptr, int num_upvalues = 0) const;

	void push(bool boolean) const;

	void push(const nil_type& n) const;

	void push(void* data) const;

	template<typename T>
	void push(const T& val) const
	{
		void* ud = create_userdata(sizeof(T));

		new(ud) T(val);

		get_table_field(registry_index(), class_id<T>());
		set_metatable(-2);
	}

	template<typename T>
	void push(T* ptr) const
	{
		push(static_cast<void*>(ptr));
	}

	template<typename T>
	T get(int idx = stack::top) const
	{
		return get_resolver<T>::get(*this, idx);
	}

#include "elba_member_functions.inc.hpp"
#include "elba_free_functions_fwd_decl.inc.hpp"

	void repush(int index) const;

	void insert(int index) const;

	void remove(int index) const;

	void pop(int num) const;

	int size() const;
	bool is_valid_index(int index) const;
	int normalize_index(int index) const;
	int upvalue_index(int index) const;
	bool are_equal(int idx1, int idx2) const;

	type element_type(int index) const;

	void* create_userdata(std::size_t size) const;
	void create_table() const;
	void create_function(const std::string& buffer) const;
	void create_function(const std::string& buffer, const std::string& name) const;

	void get_table_field(int t) const;
	void set_table_field(int t) const;

	template<typename Key, typename Value>
	void set_table_field(int t, const Key& key, const Value& value) const
	{
		push(key);
		push(value);
		set_table_field(offset_index(t, 2));
	}

	template<typename Key>
	void get_table_field(int t, const Key& key) const
	{
		push(key);
		get_table_field(offset_index(t, 1));
	}

	void raw_get_table_field(int t) const;
	void raw_set_table_field(int t) const;

	template<typename Key, typename Value>
	void raw_set_table_field(int t, const Key& key, const Value& value) const
	{
		push(key);
		push(value);
		raw_set_table_field(offset_index(t, 2));
	}

	template<typename Key>
	void raw_get_table_field(int t, const Key& key) const
	{
		push(key);
		raw_get_table_field(offset_index(t, 1));
	}

	bool get_metatable(int t) const;
	void set_metatable(int t) const;

	void call(int nargs, int nresults) const;

	bool is_of_base_type(int t, class_id_type type) const;
	bool convert_to(int t, class_id_type type) const;

	void handle_active_exception() const;
	void raise_error() const;

	static int registry_index();
	static int globals_index();

	const char* type_name(elba::stack::type t) const;
private:
	lua_State* const L;

	std::string get_element_name(int idx) const;
	std::string bound_type_name(class_id_type t) const;

	bool is_pseudo_index(int idx) const { return idx <= registry_index(); }
	int offset_index(int idx, int offset) const
	{
		if(is_pseudo_index(idx) || idx >= 0)
			return idx;

		return idx - offset;
	}

	template<typename Memfun>
	void push_memfun_wrapper(bindable_funcptr wrapper, Memfun memfun) const
	{
		push<Memfun>(memfun);
		push(wrapper, 1);
	}

	template<typename T>
	struct get_resolver
	{
		static T get(const stack& st, int idx)
		{
			if(st.element_type(idx) != stack::userdata || st.is_of_base_type(idx, class_id<T>()))
				return *(st.get<T*>(idx));

			if(st.convert_to(idx, class_id<T>()))
			{
				T tmp = *(st.get<T*>(-1));

				st.pop(1);

				return tmp;
			}

			throw elba::conversion_error(st.L, "conversion from " + st.get_element_name(idx) + " to " + st.bound_type_name(class_id<T>()) + " failed");
		}
	};
};

template<typename T>
struct stack::get_resolver<T*>
{
	static T* get(const stack& st, int idx)
	{
		if(st.element_type(idx) != stack::userdata || st.is_of_base_type(idx, class_id<T>()))
			return static_cast<T*>(st.get<void*>(idx));

		throw elba::conversion_error(st.L, "conversion from " + st.get_element_name(idx) + " to " + st.bound_type_name(class_id<T>()) + " failed");
	}
};

template<typename T>
struct stack::get_resolver<T* const>
{
	static T* get(const stack& st, int idx)
	{
		return st.get<T*>(idx);
	}
};

template<typename T>
struct stack::get_resolver<T* volatile>
{
	static T* get(const stack& st, int idx)
	{
		return st.get<T*>(idx);
	}
};

template<typename T>
struct stack::get_resolver<T* const volatile>
{
	static T* get(const stack& st, int idx)
	{
		return st.get<T*>(idx);
	}
};

template<typename T>
struct stack::get_resolver<T&>
{
	static T& get(const stack& st, int idx)
	{
		return *(st.get<T*>(idx));
	}
};

template<> std::string stack::get<std::string>(int idx) const;
template<> const char* stack::get<const char*>(int idx) const;
template<> void* stack::get<void*>(int idx) const;
template<> char stack::get<char>(int idx) const;
template<> signed char stack::get<signed char>(int idx) const;
template<> int stack::get<int>(int idx) const;
template<> short stack::get<short>(int idx) const;
template<> long stack::get<long>(int idx) const;
template<> unsigned char stack::get<unsigned char>(int idx) const;
template<> unsigned short stack::get<unsigned short>(int idx) const;
template<> unsigned int stack::get<unsigned int>(int idx) const;
template<> unsigned long stack::get<unsigned long>(int idx) const;
template<> float stack::get<float>(int idx) const;
template<> bool stack::get<bool>(int idx) const;
template<> stack::bindable_funcptr stack::get<stack::bindable_funcptr>(int idx) const;

#include "elba_free_functions.inc.hpp"

}

#endif
