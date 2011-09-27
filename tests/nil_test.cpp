#include <elba.hpp>
#include <cassert>

#define assert_equal(a, b) assert((a) == (b)), assert((b) == (a))
#define assert_not_equal(a, b) assert((a) != (b)), assert((b) != (a))

int main()
{
	elba::state lua;

	assert_not_equal(elba::nil, lua);
	assert_not_equal(elba::nil, true);
	assert_equal(elba::nil, elba::nil);
	assert(!elba::nil);
	assert_not_equal(elba::nil, lua.globals);
	assert_equal(lua.globals[1], elba::nil);
	assert_not_equal(elba::nil, 1);

	return 0;
}
