#include <elba.hpp>
#include <iostream>

struct B
{
	B() : truth(false) {}

	bool truth;
};

struct A
{
	A() : a(1) { std::cout << "initializing" << std::endl; }
	~A() { std::cout << "finalizing" << std::endl; }

	int assess_truth(B b) const { return b.truth ? a : 0; }
	bool determine(B b) { return b.truth; }

	int a;
};

int main()
{
	elba::state L;
	L.open_libs();

	L.globals["A"] = L.bind_class<A>()
		.constructor<A>()
		.method("assess_truth", &A::assess_truth)
		.method("determine", &A::determine);

	L.globals["B"] = L.bind_class<B>()
		.constructor<B>();

	L.do_string("a = A.new()\nb = B.new()\nprint(a:assess_truth(b))\nprint(not a:determine(b))");

	std::cout << L.globals["a"]["assess_truth"](L.globals["a"], L.globals["b"]) << std::endl;
}
