#include <elba_nil.hpp>
#include <elba_reference.hpp>

namespace elba
{

const nil_type nil;

bool operator==(const nil_type& n, const reference& ref) { return ref.type() == stack::nil; }

}