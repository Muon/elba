#ifndef ELBA_STACK_HPP
#define ELBA_STACK_HPP

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
template<typename T> class_id_type class_id() { static const char c = 0; return reinterpret_cast<class_id_type>(const_cast<char*>(&c)); }

struct nil_type;

class stack
{
public:
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

	void push(signed char integer) const;
	void push(short integer) const;
	void push(int integer) const;
	void push(long integer) const;

	void push(unsigned char c) const;
	void push(unsigned short integer) const;
	void push(unsigned int integer) const;
	void push(unsigned long integer) const;

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

	void raw_get_table_field(int t) const;
	void raw_set_table_field(int t) const;

	bool get_metatable(int t) const;
	void set_metatable(int t) const;

	void call(int nargs, int nresults) const;

	void handle_active_exception() const;
	void raise_error() const;

	static int registry_index();
	static int globals_index();
private:
	lua_State* const L;

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
			T* p = st.get<T*>(idx);
			return *p;
		}
	};
};

template<typename T>
struct stack::get_resolver<T*>
{
	static T* get(const stack& st, int idx)
	{
		return static_cast<T*>(st.get<void*>(idx));
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
