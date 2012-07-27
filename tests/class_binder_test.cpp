#include "elba_test.hpp"

extern "C"
{
#include "lua.h"
}

struct Bar {};

struct Foo
{
	Foo() { ++count_constructed; }
	Foo(const Foo&) { ++count_constructed; }
	~Foo() { --count_constructed; }

	void f() {}
	operator Bar() const { return Bar(); }

	static int count_constructed;
};

int Foo::count_constructed = 0;

class ClassBinderTest : public TestBase
{
public:
	ClassBinderTest() : globals(state.globals())
	{
		elba::class_binder<Foo>(state, "Foo")
			.constructor()
			.set("bar", "baz")
			.set("f", &Foo::f)
			.conversion_operator<Bar>();
	}

	~ClassBinderTest()
	{
		lua_gc(state, LUA_GCCOLLECT, 0);
		EXPECT_EQ(0, Foo::count_constructed);
	}

	elba::reference globals;
};

TEST_F(ClassBinderTest, BoundName)
{
	elba::reference x(state);
	x = Foo();
	EXPECT_EQ(1, Foo::count_constructed);
	EXPECT_EQ("Foo", x.metatable()["name"]);
}

TEST_F(ClassBinderTest, HasRegisteredStaticTable)
{
	elba::reference statics = globals.get<elba::reference>("Foo");
	EXPECT_EQ(elba::stack::table, statics.type());
}

TEST_F(ClassBinderTest, DefaultStatics)
{
	elba::reference statics = globals.get<elba::reference>("Foo");
	EXPECT_EQ(elba::stack::function, statics.get<elba::reference>("new").type());
	EXPECT_EQ("baz", statics.get<elba::reference>("bar"));
}

TEST_F(ClassBinderTest, Construction)
{
	elba::reference x = globals.get<elba::reference>("Foo").get<elba::reference>("new")();
	EXPECT_EQ(1, Foo::count_constructed);
}
