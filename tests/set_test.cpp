#include <elba.hpp>

int main()
{
	elba::state lua;

	lua.open_libs();

	lua.globals["greeting"] = "Hello, world!";
	lua.globals["num"] = 15;
	
	lua.do_string("print(greeting)\nprint(\"You are number: \", num);");

	return 0;
}
