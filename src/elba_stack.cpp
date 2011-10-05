#include <elba_stack.hpp>
#include <elba_reference.hpp>
#include <elba_classbinder.hpp>

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <cassert>

// copied verbatim from Lua's source; see Lua 5.1 license for copyright info
// needed for tostring stringification semantics
static int luaB_tostring (lua_State *L) {
  luaL_checkany(L, 1);
  if (luaL_callmeta(L, 1, "__tostring"))  /* is there a metafield? */
    return 1;  /* use its value */
  switch (lua_type(L, 1)) {
    case LUA_TNUMBER:
      lua_pushstring(L, lua_tostring(L, 1));
      break;
    case LUA_TSTRING:
      lua_pushvalue(L, 1);
      break;
    case LUA_TBOOLEAN:
      lua_pushstring(L, (lua_toboolean(L, 1) ? "true" : "false"));
      break;
    case LUA_TNIL:
      lua_pushliteral(L, "nil");
      break;
    default:
      lua_pushfstring(L, "%s: %p", luaL_typename(L, 1), lua_topointer(L, 1));
      break;
  }
  return 1;
}

namespace elba
{

void stack::push(const std::string& string) const
{
	lua_pushlstring(L, string.data(), string.size());
}

template<>
std::string stack::get<std::string>(int idx) const
{
	idx = normalize_index(idx);
	push(luaB_tostring);
	repush(idx);
	call(1, 1);

	std::string::size_type len;
	const char* str = lua_tolstring(L, -1, &len);
	std::string tmp(str, len);

	pop(1);

	return tmp;
}


void stack::push(const char* string) const
{
	lua_pushstring(L, string);
}

template<>
const char* stack::get<const char*>(int idx) const
{
	return lua_tostring(L, idx);
}

void stack::push(char c) const
{
	lua_pushlstring(L, &c, sizeof c);
}

template<>
char stack::get<char>(int idx) const
{
	return get<const char*>(idx)[0];
}

void stack::push(short integer) const
{
	lua_pushinteger(L, integer);
}

template<>
short stack::get<short>(int idx) const
{
	return lua_tointeger(L, idx);
}

void stack::push(int integer) const
{
	lua_pushinteger(L, integer);
}

template<>
int stack::get<int>(int idx) const
{
	return lua_tointeger(L, idx);
}

void stack::push(long int integer) const
{
	lua_pushinteger(L, integer);
}

template<>
long stack::get<long>(int idx) const
{
	return lua_tointeger(L, idx);
}

void stack::push(unsigned char c) const
{
	lua_pushinteger(L, c);
}

template<>
unsigned char stack::get<unsigned char>(int idx) const
{
	return lua_tointeger(L, idx);
}

void stack::push(unsigned short integer) const
{
	lua_pushinteger(L, integer);
}

template<>
unsigned short stack::get<unsigned short>(int idx) const
{
	return lua_tointeger(L, idx);
}

void stack::push(unsigned int integer) const
{
	lua_pushinteger(L, integer);
}

template<>
unsigned int stack::get<unsigned int>(int idx) const
{
	return lua_tointeger(L, idx);
}

void stack::push(unsigned long int integer) const
{
	lua_pushinteger(L, integer);
}

template<>
unsigned long stack::get<unsigned long>(int idx) const
{
	return lua_tointeger(L, idx);
}

void stack::push(double number) const
{
	lua_pushnumber(L, number);
}

template<>
double stack::get<double>(int idx) const
{
	return lua_tonumber(L, idx);
}


void stack::push(float number) const
{
	lua_pushnumber(L, number);
}

template<>
float stack::get<float>(int idx) const
{
	return lua_tonumber(L, idx);
}

void stack::pop(int num) const
{
	lua_pop(L, num);
}

void stack::push(const stack::bindable_funcptr ptr, int num_upvalues) const
{
	lua_pushcclosure(L, ptr, num_upvalues);
}

void stack::push(bool boolean) const
{
	lua_pushboolean(L, boolean);
}

template<>
bool stack::get<bool>(int idx) const
{
	return lua_toboolean(L, idx);
}

template<>
reference stack::get<reference>(int idx) const
{
	return reference(L, idx);
}

void stack::push(const nil_type& n) const
{
	static_cast<void>(n);
	lua_pushnil(L);
}

void stack::push(void* data) const
{
	lua_pushlightuserdata(L, data);
}

template<>
void* stack::get<void*>(int idx) const
{
	return lua_touserdata(L, idx);
}

template<>
stack::bindable_funcptr stack::get<stack::bindable_funcptr>(int idx) const
{
	return lua_tocfunction(L, idx);
}

void stack::push(void (*func_ptr)()) const
{
	struct wrapper_creator
	{
		typedef void (*wrapped_funcptr)();

		static int wrapper(lua_State* L)
		{
			stack st(L);

			wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get<bindable_funcptr>(st.upvalue_index(1)));

			function_real();

			return 0;
		}
	};

	push(reinterpret_cast<bindable_funcptr>(func_ptr));
	push(wrapper_creator::wrapper, 1);
}

void stack::repush(int index) const
{
	lua_pushvalue(L, index);
}

int stack::size() const
{
	return lua_gettop(L);
}

bool stack::is_valid_index(int index) const
{
	return !lua_isnone(L, index);
}

int stack::normalize_index(int index) const
{
	assert(index != 0);

	if(index > 0)
		return index;

	return size() + index + 1;
}

int stack::upvalue_index(int index) const
{
	return lua_upvalueindex(index);
}

bool stack::are_equal(int idx1, int idx2) const
{
	return lua_equal(L, idx1, idx2);
}

stack::type stack::element_type(int index) const
{
	return static_cast<stack::type>(lua_type(L, index));
}

reference stack::create_userdata(std::size_t size) const
{
	void* data = lua_newuserdata(L, size);
	assert(data != NULL);
	reference ud(L, stack::top);
	pop(1);

	return ud;
}

reference stack::create_table() const
{
	lua_newtable(L);
	reference t(L, stack::top);
	pop(1);

	return t;
}

void stack::get_table_field(int t) const
{
	lua_gettable(L, t);
}

void stack::set_table_field(int t) const
{
	lua_settable(L, t);
}

void stack::raw_get_table_field(int t) const
{
	lua_rawget(L, t);
}

void stack::raw_set_table_field(int t) const
{
	lua_rawset(L, t);
}

void stack::call(int nargs, int nresults) const
{
	lua_call(L, nargs, nresults);
}

}
