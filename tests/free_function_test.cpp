#include <elba.hpp>
#include <iostream>
#include <string>

unsigned int f(unsigned int a)
{
	return a;
}

const char* g(const char* str)
{
	return str;
}

void h(int a)
{
	static_cast<void>(a);
}

int main()
{
	elba::state L;
	elba::reference globals = L.globals();
	L.open_libs();

	globals["f"] = f;
	globals["g"] = g;
	globals["h"] = h;

	L.do_string("print(f(-12))\nprint(g(\"I am bound!\"))\nprint(h())");

	elba::reference print = globals["print"];

	int p = print("I am being called from C++!");
	p = print("I am being called from C++, too!");

	short t = print("barley");
	t = print("hot chocolate");

	elba::reference tab = print("coconut");
	tab = print("cheese");

	print("apple", "granny smith", "crab apple", "pineapple?!?!");

	std::string str = print("lemon");
	str = print("orange").get<std::string>();

	return 0;
}
