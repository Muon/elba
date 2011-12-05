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

#define MAKE_STACK_GETTER(type, expr) template<> type stack::get<type>(int idx) const { return expr; }
MAKE_STACK_GETTER(long, lua_tointeger(L, idx))
MAKE_STACK_GETTER(unsigned long, get<long>(idx))
MAKE_STACK_GETTER(double, lua_tonumber(L, idx))
MAKE_STACK_GETTER(float, get<number_type>(idx))
MAKE_STACK_GETTER(bool, lua_toboolean(L, idx))
MAKE_STACK_GETTER(void*, lua_touserdata(L, idx))
MAKE_STACK_GETTER(const char*, lua_tostring(L, idx))
MAKE_STACK_GETTER(char, get<const char*>(idx)[0])
MAKE_STACK_GETTER(stack::bindable_funcptr, lua_tocfunction(L, idx))
#undef MAKE_STACK_GETTER

#define MAKE_STACK_PUSHER(type, expr) void stack::push(type value) const { expr; }
MAKE_STACK_PUSHER(const std::string&, lua_pushlstring(L, value.data(), value.size()))
MAKE_STACK_PUSHER(const char*, lua_pushstring(L, value))
MAKE_STACK_PUSHER(char, lua_pushlstring(L, &value, 1))
MAKE_STACK_PUSHER(long, lua_pushinteger(L, value))
MAKE_STACK_PUSHER(double, lua_pushnumber(L, value))
MAKE_STACK_PUSHER(float, push(static_cast<double>(value)))
MAKE_STACK_PUSHER(bool, lua_pushboolean(L, value))
MAKE_STACK_PUSHER(const nil_type&, lua_pushnil(L))
MAKE_STACK_PUSHER(void*, lua_pushlightuserdata(L, value))
#undef MAKE_STACK_PUSHER

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

void stack::push(const stack::bindable_funcptr ptr, int num_upvalues) const
{
	lua_pushcclosure(L, ptr, num_upvalues);
}

void stack::pop(int num) const
{
	lua_pop(L, num);
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

	if(index <= registry_index())
		return index;

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
			// we're not binding elba::runtime_error so we have to
			// avoid the checking mechanism
			runtime_error err = *(static_cast<runtime_error*>(get<void*>(-1)));
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

bool stack::is_of_base_type(int t, class_id_type type) const
{
	if(get_metatable(t))
	{
		get_table_field(-1, "types");
		get_table_field(-1, type);

		bool result = get<bool>(-1);

		pop(3);

		return result;
	}

	return false;
}

bool stack::convert_to(int t, class_id_type type) const
{
	if(get_metatable(t))
	{
		get_table_field(-1, "convops");
		get_table_field(-1, type);

		if(element_type(-1) == ::elba::stack::function)
		{
			repush(t);
			call(1, 1);
			insert(-3);
			pop(2);

			return true;
		}

		pop(3);
	}

	return false;
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

int stack::registry_index()
{
	return LUA_REGISTRYINDEX;
}

int stack::globals_index()
{
	return LUA_GLOBALSINDEX;
}

const char* stack::type_name(type t) const
{
	return lua_typename(L, t);
}

std::string stack::get_element_name(int idx) const
{
	if(element_type(idx) == userdata)
	{
		if(get_metatable(idx))
		{
			push("name");
			get_table_field(-2);

			std::string tmp = get<std::string>(-1);

			pop(2);

			return tmp;
		}

		return "(unknown)";
	}

	return type_name(element_type(idx));
}

std::string stack::bound_type_name(class_id_type t) const
{
	get_table_field(registry_index(), t);
	if(element_type(-1) == ::elba::stack::nil)
	{
		pop(1);
		return "(unknown)";
	}

	get_table_field(-1, "name");

	std::string name = get<std::string>(-1);

	pop(2);

	return name;
}

}
