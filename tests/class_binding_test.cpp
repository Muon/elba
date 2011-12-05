#include <elba.hpp>
#include <iostream>
#include <stdexcept>

struct B
{
	B() : truth(false) {}
	B(const B& b) : truth(b.truth) { std::cout << "copying B" << std::endl; }

	void examine(B x) { std::cout << "examining " << x.truth << std::endl; if(x.truth) throw std::runtime_error("the truth is true"); }

	bool truth;
};

struct A
{
	A() : a(1) { std::cout << "initializing" << std::endl; }
	~A() { std::cout << "finalizing" << std::endl; }

	operator B() { B tmp; std::cout << "converting to B" << std::endl; tmp.truth = a; return tmp; }

	int assess_truth(B b) const { return b.truth ? a : 0; }
	bool determine(const B& b) { return b.truth; }
	void modify(B& b) { std::cout << "modifying" << std::endl; b.truth = !b.truth; }

	int a;
};

int main()
{
	elba::state L;
	L.open_libs();

	elba::class_binder<A>(L, "A")
		.constructor()
		.set("assess_truth", &A::assess_truth)
		.set("determine", &A::determine)
		.set("modify", &A::modify)
		.conversion_operator<B>();

	elba::class_binder<B>(L, "B")
		.constructor()
		.set("examine", &B::examine);

	L.do_string("a = A.new()\nb = B.new()\nprint(a:assess_truth(a))\nb:examine(a)\na:modify(b)\nprint(not a:determine(b))");

	elba::reference globals = L.globals();
	std::cout << globals["a"]["assess_truth"](globals["a"], globals["b"]) << std::endl;
}
