#ifndef ELBA_STACK_HPP
#define ELBA_STACK_HPP

#include "elba_error.hpp"

#include <boost/type_traits/remove_cv.hpp>

#include <string>

struct lua_State;

namespace elba
{

template<typename T> struct clean_type { typedef typename boost::remove_cv<T>::type type; };

typedef void* class_id_type;
template<typename T> class_id_type class_id_() { static const char c = 0; return reinterpret_cast<class_id_type>(const_cast<char*>(&c)); }

template<typename T> class_id_type class_id() { return class_id_<typename clean_type<T>::type>(); }

struct nil_type;

class stack
{
public:
	typedef long integer_type;
	typedef unsigned long unsigned_integer_type;
	typedef double number_type;

	typedef int(*bindable_funcptr)(lua_State*);

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

	void push(long integer) const;
#define ELBA_DEFINE_INTEGER_PUSHER(type) void push(type integer) const { push(static_cast<integer_type>(integer)); }
	ELBA_DEFINE_INTEGER_PUSHER(signed char)
	ELBA_DEFINE_INTEGER_PUSHER(short)
	ELBA_DEFINE_INTEGER_PUSHER(int)
	ELBA_DEFINE_INTEGER_PUSHER(unsigned char)
	ELBA_DEFINE_INTEGER_PUSHER(unsigned short)
	ELBA_DEFINE_INTEGER_PUSHER(unsigned int)
	ELBA_DEFINE_INTEGER_PUSHER(unsigned long)
#undef ELBA_DEFINE_INTEGER_PUSHER

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

		get_table_field(registry_index, class_id<T>());
		set_metatable(-2);
	}

	template<typename T>
	void push(T* ptr) const
	{
		push_typed_pointer(ptr, class_id<T>());
	}

	template<typename T>
	T get(int idx = -1) const
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

	bool is_of_bound_type(int t, class_id_type type) const;
	bool convert_to(int t, class_id_type type) const;

	void handle_active_exception() const;
	void raise_error() const;

	static const int registry_index;
	static const int globals_index;

	const char* type_name(elba::stack::type t) const;
private:
	lua_State* const L;

	std::string get_element_name(int idx) const;
	std::string bound_type_name(class_id_type t) const;

	bool is_pseudo_index(int idx) const { return idx <= registry_index; }
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

	void push_typed_pointer(void* ptr, class_id_type type) const;

	template<typename T>
	struct get_resolver
	{
		static T get(const stack& st, int idx)
		{
			if(st.is_of_bound_type(idx, class_id<T>()))
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
		if(st.is_of_bound_type(idx, class_id<T>()))
		{
			if(st.element_type(idx) == table)
			{
				st.get_table_field(idx, "__self");
				T* tmp = static_cast<T*>(st.get<void*>(-1));
				st.pop(1);
				return tmp;
			}
			else
			{
				return static_cast<T*>(st.get<void*>(idx));
			}
		}

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

#define ELBA_DECLARE_STACK_GETTER(type) template<> type stack::get<type>(int idx) const;
ELBA_DECLARE_STACK_GETTER(char)
ELBA_DECLARE_STACK_GETTER(const char*)
ELBA_DECLARE_STACK_GETTER(std::string)

ELBA_DECLARE_STACK_GETTER(void*)

ELBA_DECLARE_STACK_GETTER(long)
ELBA_DECLARE_STACK_GETTER(unsigned long)

ELBA_DECLARE_STACK_GETTER(double)
ELBA_DECLARE_STACK_GETTER(float)
ELBA_DECLARE_STACK_GETTER(bool)
ELBA_DECLARE_STACK_GETTER(stack::bindable_funcptr)
#undef ELBA_DECLARE_STACK_GETTER

#define ELBA_INLINE_STACK_GETTER(type) template<> inline type stack::get<type>(int idx) const
#define ELBA_INTEGER_STACK_GETTER(type) ELBA_INLINE_STACK_GETTER(type) { return get<integer_type>(idx); }
ELBA_INTEGER_STACK_GETTER(signed char)
ELBA_INTEGER_STACK_GETTER(short)
ELBA_INTEGER_STACK_GETTER(int)
#undef ELBA_INTEGER_STACK_GETTER

#define ELBA_UINTEGER_STACK_GETTER(type) ELBA_INLINE_STACK_GETTER(type) { return get<unsigned_integer_type>(idx); }
ELBA_UINTEGER_STACK_GETTER(unsigned char)
ELBA_UINTEGER_STACK_GETTER(unsigned short)
ELBA_UINTEGER_STACK_GETTER(unsigned int)
#undef ELBA_UINTEGER_STACK_GETTER
#undef ELBA_INLINE_STACK_GETTER

#include "elba_free_functions.inc.hpp"

}

#endif
