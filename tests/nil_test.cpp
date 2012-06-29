#include <elba.hpp>
#include "test_utilities.hpp"

int main()
{
	elba::state lua;
	elba::reference globals = lua.globals();

	assert_not_equal(elba::nil, globals);
	assert_equal(globals[1], elba::nil);

	return 0;
}
