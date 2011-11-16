#include <elba_stack.hpp>
#include <elba_error.hpp>

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <cassert>
#include <stdexcept>

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

void stack::push(signed char integer) const
{
	lua_pushinteger(L, integer);
}

template<>
signed char stack::get<signed char>(int idx) const
{
	return lua_tointeger(L, idx);
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

void stack::repush(int index) const
{
	lua_pushvalue(L, index);
}

void stack::insert(int index) const
{
	lua_insert(L, index);
}

void stack::remove(int index) const
{
	lua_remove(L, index);
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

void* stack::create_userdata(std::size_t size) const
{
	void* data = lua_newuserdata(L, size);
	assert(data != NULL);

	return data;
}

void stack::create_table() const
{
	lua_newtable(L);
}

void stack::get_table_field(int t) const
{
	lua_gettable(L, t);
}

void stack::create_function(const std::string& buffer) const
{
	create_function(buffer, buffer);
}

void stack::create_function(const std::string& buffer, const std::string& name) const
{
	int ret = luaL_loadbuffer(L, buffer.data(), buffer.size(), name.c_str());
	if(ret != 0)
	{
		if(ret == LUA_ERRSYNTAX)
		{
			std::string msg = get<const char*>(-1);
			pop(1);

			throw syntax_error(L, msg);
		}
		else if(ret == LUA_ERRMEM)
		{
			pop(1);
			throw out_of_memory_error(L);
		}
		else
			assert(false);
	}
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

static int error_handler(lua_State* L)
{
	stack st(L);

	st.push(runtime_error(L, st.get<const char*>(1)));

	return 1;
}

bool stack::get_metatable(int t) const
{
	return lua_getmetatable(L, t);
}

void stack::set_metatable(int t) const
{
	lua_setmetatable(L, t);
}

void stack::call(int nargs, int nresults) const
{
	int sz = size();

	push(error_handler);
	insert(-nargs - 2);

	int ret = lua_pcall(L, nargs, nresults, -nargs - 2);

	if(ret == 0)
	{
		remove(-nresults - 1);
	}
	else
	{
		if(ret == LUA_ERRRUN)
		{
			runtime_error err = get<runtime_error>(-1);
			pop(2);

			throw err;
		}
		else
		{
			if(ret == LUA_ERRMEM)
			{
				pop(2);
				throw out_of_memory_error(L);
			}
			else if(ret == LUA_ERRERR)
			{
				std::string msg = get<const char*>(-1);
				pop(2);

				throw error_handler_error(L, msg);
			}
			else
				assert(false);
		}
	}

	assert(size() == sz - (nargs + 1) + nresults);
}

void stack::handle_active_exception() const
{
	try
	{
		throw;
	}
	catch(const std::exception& e)
	{
		push(e.what());
	}
	catch(...)
	{
		push("unknown C++ exception caught");
	}
}

void stack::raise_error() const
{
	lua_error(L);
}

}
