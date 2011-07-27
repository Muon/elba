#include <elba.hpp>

#include <iostream>

#include <string>

int main()
{
	elba::state lua;

	lua.open_libs();

	lua.globals["greeting"] = "Hello, world!";
	lua.globals["num"] = 15;
	lua.globals["float"] = 1.5f;
	lua.globals["truth"] = true;

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

	std::string tmp = lua.globals["greeting"];
	std::cout << "The greeting is \"" << tmp << "\"!" << std::endl;
	std::cout << "The number is " << int(lua.globals["num"]) << std::endl;
	std::cout << "The float is " << float(lua.globals["float"]) << std::endl;

	return 0;
}
