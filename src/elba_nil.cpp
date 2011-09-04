#include <elba_nil.hpp>
#include <elba_reference.hpp>

namespace elba
{

const nil_type nil;

bool nil_type::operator==(const reference& ref) const { return ref == *this; }
bool nil_type::operator==(const object_index& idx) const { return idx.operator==(*this); }

}
