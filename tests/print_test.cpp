#include <elba.hpp>

int main()
{
	elba::state lua;

	lua.open_libs();

	lua.do_string("print(3*2)");

	return 0;
}
