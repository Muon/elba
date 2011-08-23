#include <elba.hpp>
#include <iostream>

struct A
{
	A() : a(1) { std::cout << "initializing" << std::endl; }
	~A() { std::cout << "finalizing" << std::endl; }

	int get() const { return a; }

	int a;
};

int main()
{
	elba::state L;
	L.open_libs();

	elba::class_binder binder(L);

	L.globals["A"] = L.bind_class<A>()
		.constructor<A>()
		.method("get", &A::get);

	L.do_string("a = A.new()\nprint(a)\nprint(a.get)\nprint(a:get())");

}