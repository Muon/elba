#include <elba.hpp>

int main()
{
	elba::interpreter lua;

	lua.openLibs();

	lua.doString("print(3*2)");
	
	return 0;
}
