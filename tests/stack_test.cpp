#include "elba_test.hpp"

class StackTest : public TestBase
{
};

const char* f() { return "foo"; }

TEST_F(StackTest, PushNullaryNonvoidFreeFunction)
{
	st.push(f);
	st.call(0, 1);
	EXPECT_STREQ("foo", st.get<const char*>(-1));
	st.pop(1);
}

const char* g(const char* s) { EXPECT_STREQ("foo", s); return s; }

TEST_F(StackTest, PushUnaryNonvoidFreeFunction)
{
	st.push(g);
	st.push("foo");
	st.call(1, 1);
	EXPECT_STREQ("foo", st.get<const char*>(-1));
	st.pop(1);
}

static bool h_called = false;
void h() { h_called = true; }

TEST_F(StackTest, PushNullaryVoidFreeFunction)
{
	st.push(h);
	st.call(0, 0);
	EXPECT_TRUE(h_called);
	h_called = false;
}

static const char* i_string_passed = NULL;
void i(const char* s) { i_string_passed = s; }

TEST_F(StackTest, PushUnaryVoidFreeFunction)
{
	st.push(i);
	st.push("foo");
	st.call(1, 0);
	EXPECT_STREQ("foo", i_string_passed);
	i_string_passed = NULL;
}

struct A
{
	A() : self(this), sentinel(0xDEADBEEF) {}

	void check_preconditions() const { EXPECT_EQ(self, this); EXPECT_EQ(0xDEADBEEF, sentinel); }

	const char* f() { check_preconditions(); return "foo"; }
	const char* g() const { check_preconditions(); return "bar"; }
	void h() { check_preconditions(); sentinel = 0xCAFEBABE; }
	void i() const { check_preconditions(); sentinel = 0xCAFED00D; }

	const char* j(const char* s) { check_preconditions(); return s; }
	const char* k(const char* s) const { check_preconditions(); return s; }
	void l(const char* s) { check_preconditions(); EXPECT_STREQ("foo", s); sentinel = 0xCAFEBABE; }
	void m(const char* s) const { check_preconditions(); EXPECT_STREQ("bar", s); sentinel = 0xCAFED00D; }

	A* const self;
	mutable int sentinel;
};

class MemFunStackTest : public StackTest
{
public:
	MemFunStackTest()
	{
		st.push(elba::class_id<A>());
		st.create_table();
		st.set_table_field(-1, "name", "A");
		st.set_table_field(st.registry_index());

		st.get_table_field(st.registry_index(), elba::class_id<A>());
		EXPECT_EQ(elba::stack::table, st.element_type(-1));
		st.pop(1);
	}

	A tmp;

	template<typename T>
	void TestImproperMemFunCall(const T& memfun)
	{
		st.push(memfun);
		st.push("foo");
		ASSERT_THROW(st.call(1, 1), elba::runtime_error);
	}
};

#define TEST_IMPROPER(test, memfun) TEST_F(MemFunStackTest, ImproperlyCall ## test) { TestImproperMemFunCall(&A::memfun); }

TEST_F(MemFunStackTest, PushNullaryNonvoidNonconstMemberFunction)
{
	st.push(&A::f);
	st.push(&tmp);
	st.call(1, 1);
	EXPECT_STREQ("foo", st.get<const char*>(-1));
	st.pop(1);
}

TEST_IMPROPER(NullaryNonvoidNonconstMemberFunction, f)

TEST_F(MemFunStackTest, PushNullaryNonvoidConstMemberFunction)
{
	st.push(&A::g);
	st.push(&tmp);
	st.call(1, 1);
	EXPECT_STREQ("bar", st.get<const char*>(-1));
	st.pop(1);
}

TEST_IMPROPER(NullaryNonvoidConstMemberFunction, g)

TEST_F(MemFunStackTest, PushNullaryVoidNonconstMemberFunction)
{
	st.push(&A::h);
	st.push(&tmp);
	st.call(1, 0);
	EXPECT_EQ(0xCAFEBABE, tmp.sentinel);
}

TEST_IMPROPER(NullaryVoidNonconstMemberFunction, h)

TEST_F(MemFunStackTest, PushNullaryVoidConstMemberFunction)
{
	st.push(&A::i);
	st.push(&tmp);
	st.call(1, 0);
	EXPECT_EQ(0xCAFED00D, tmp.sentinel);
}

TEST_IMPROPER(NullaryVoidConstMemberFunction, i)

TEST_F(MemFunStackTest, PushUnaryNonvoidNonconstMemberFunction)
{
	st.push(&A::j);
	st.push(&tmp);
	st.push("foo");
	st.call(2, 1);
	EXPECT_STREQ("foo", st.get<const char*>(-1));
	st.pop(1);
}

TEST_IMPROPER(UnaryNonvoidNonconstMemberFunction, j)

TEST_F(MemFunStackTest, PushUnaryNonvoidConstMemberFunction)
{
	st.push(&A::k);
	st.push(&tmp);
	st.push("bar");
	st.call(2, 1);
	EXPECT_STREQ("bar", st.get<const char*>(-1));
	st.pop(1);
}

TEST_IMPROPER(UnaryNonvoidConstMemberFunction, k)

TEST_F(MemFunStackTest, PushUnaryVoidNonconstMemberFunction)
{
	st.push(&A::l);
	st.push(&tmp);
	st.push("foo");
	st.call(2, 0);
	EXPECT_EQ(0xCAFEBABE, tmp.sentinel);
}

TEST_IMPROPER(UnaryVoidNonconstMemberFunction, l)

TEST_F(MemFunStackTest, PushUnaryVoidConstMemberFunction)
{
	st.push(&A::m);
	st.push(&tmp);
	st.push("bar");
	st.call(2, 0);
	EXPECT_EQ(0xCAFED00D, tmp.sentinel);
}

TEST_IMPROPER(UnaryVoidConstMemberFunction, m)
