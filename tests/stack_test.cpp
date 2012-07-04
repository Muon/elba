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

TEST_F(StackTest, PushNullaryNonvoidNonconstMemberFunction)
{
	st.push(&A::f);
	A tmp;
	st.push(&tmp);
	st.call(1, 1);
	EXPECT_STREQ("foo", st.get<const char*>(-1));
	st.pop(1);
}

TEST_F(StackTest, PushNullaryNonvoidConstMemberFunction)
{
	st.push(&A::g);
	A tmp;
	st.push(&tmp);
	st.call(1, 1);
	EXPECT_STREQ("bar", st.get<const char*>(-1));
	st.pop(1);
}

TEST_F(StackTest, PushNullaryVoidNonconstMemberFunction)
{
	st.push(&A::h);
	A tmp;
	st.push(&tmp);
	st.call(1, 0);
	EXPECT_EQ(0xCAFEBABE, tmp.sentinel);
}

TEST_F(StackTest, PushNullaryVoidConstMemberFunction)
{
	st.push(&A::i);
	A tmp;
	st.push(&tmp);
	st.call(1, 0);
	EXPECT_EQ(0xCAFED00D, tmp.sentinel);
}

TEST_F(StackTest, PushUnaryNonvoidNonconstMemberFunction)
{
	st.push(&A::j);
	A tmp;
	st.push(&tmp);
	st.push("foo");
	st.call(2, 1);
	EXPECT_STREQ("foo", st.get<const char*>(-1));
	st.pop(1);
}

TEST_F(StackTest, PushUnaryNonvoidConstMemberFunction)
{
	st.push(&A::k);
	A tmp;
	st.push(&tmp);
	st.push("bar");
	st.call(2, 1);
	EXPECT_STREQ("bar", st.get<const char*>(-1));
	st.pop(1);
}

TEST_F(StackTest, PushUnaryVoidNonconstMemberFunction)
{
	st.push(&A::l);
	A tmp;
	st.push(&tmp);
	st.push("foo");
	st.call(2, 0);
	EXPECT_EQ(0xCAFEBABE, tmp.sentinel);
}

TEST_F(StackTest, PushUnaryVoidConstMemberFunction)
{
	st.push(&A::m);
	A tmp;
	st.push(&tmp);
	st.push("bar");
	st.call(2, 0);
	EXPECT_EQ(0xCAFED00D, tmp.sentinel);
}
