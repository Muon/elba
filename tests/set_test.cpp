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
	elba::reference globals = lua.globals();

	lua.open_libs();

	globals["greeting"] = "Hello, world!";
	globals["num"] = 15;
	globals["float"] = 1.5f;
	globals["truth"] = true;

	A x(1);
	elba::class_binder<A>(lua, "A");
	globals["struct"] = A(2);
	globals["struct1"] = &x;
	A ref = globals["struct"];
	A* ref1 = globals["struct"];
	A* ref2 = globals["struct1"];
	std::cout << "The magic number is " << ref.v << ", " << ref1->v << std::endl;
	std::cout << "More magic: " << ref2->v << std::endl;

	globals.set("foo", lua.create_table());
	elba::reference t = globals["foo"];
	t["bar"] = "baz";

	globals["one"] = lua.create_table();
	globals["one"]["two"] = "three";

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

	std::cout << "The greeting is \"" << globals["greeting"] << "\"!" << std::endl;
	std::cout << "The number is " << globals["num"] << std::endl;
	std::cout << "The float is " << globals["float"] << std::endl;

	return 0;
}
