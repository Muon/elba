#include <elba.hpp>
#include "test_utilities.hpp"

int main()
{
	elba::state lua;

	assert_not_equal(elba::nil, lua);
	assert_not_equal(elba::nil, true);
	assert_equal(elba::nil, elba::nil);
	assert_false(elba::nil);
	assert_not_equal(elba::nil, lua.globals);
	assert_equal(lua.globals[1], elba::nil);
	assert_not_equal(elba::nil, 1);

	return 0;
}
