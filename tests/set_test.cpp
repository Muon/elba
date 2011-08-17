#include <elba.hpp>

#include <iostream>

#include <string>

struct A
{
	A() : v(123) {}
	A(int x) : v(x) {}
	
	int v;
};

int main()
{
	elba::state lua;

	lua.open_libs();

	lua.globals["greeting"] = "Hello, world!";
	lua.globals["num"] = 15;
	lua.globals["float"] = 1.5f;
	lua.globals["truth"] = true;
	
	A x(1);
	lua.globals["struct"] = A(2);
	lua.globals["struct1"] = &x;
	A ref = lua.globals["struct"];
	A* ref1 = lua.globals["struct"];
	A* ref2 = lua.globals["struct1"];
	std::cout << "The magic number is " << ref.v << ", " << ref1->v << std::endl;
	std::cout << "More magic: " << ref2->v << std::endl;

	lua.globals.set("foo", elba::table(lua));
	elba::table t = lua.globals["foo"];
	t["bar"] = "baz";

	lua.globals["one"] = elba::table(lua);
	lua.globals["one"]["two"] = "three";

	lua.do_string(
		"print(greeting)\n"
		"print(\"You are number: \" .. num)\n"
		"greeting = \"Salutations, world!\""
		"num = 42\n"
		"print(float + 12.3)\n"
		"print(\"The truth is \" .. (truth and \"true\" or \"false\"))\n"
		"print(foo.bar)\n"
		"print(one.two)\n"
	);

	std::cout << "The greeting is \"" << lua.globals["greeting"] << "\"!" << std::endl;
	std::cout << "The number is " << lua.globals["num"] << std::endl;
	std::cout << "The float is " << lua.globals["float"] << std::endl;

	return 0;
}
