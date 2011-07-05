#include <elba.hpp>

int f(int a)
{
	return a;
}

const char* g(const char* str)
{
	return str;
}

void h(int a)
{
}

int main()
{
	elba::state L;
	L.open_libs();

	L.globals["f"] = f;
	L.globals["g"] = g;
	L.globals["h"] = h;

	L.do_string("print(f(12))\nprint(g(\"I am bound!\"))\nprint(h())");

	return 0;
}
