#include <elba.hpp>
#include <cassert>

int main()
{
	elba::state lua;

	assert(elba::nil != lua);
	assert(elba::nil != true);
	assert(elba::nil == elba::nil);
	assert(!elba::nil);

	return 0;
}
