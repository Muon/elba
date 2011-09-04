#ifndef ELBA_NIL_HPP
#define ELBA_NIL_HPP

namespace elba
{

class reference;
class object_index;

struct nil_type
{
	nil_type() {}

	operator bool() const { return false; }

	bool operator==(const nil_type& n) const { static_cast<void>(n); return true; }
	bool operator==(const reference& ref) const;
	bool operator==(const object_index& idx) const;
};

template<typename T> bool operator==(const nil_type& n, const T& val) { static_cast<void>(n); static_cast<void>(val); return false; }
template<typename T> bool operator!=(const nil_type& n, const T& val) { return !(n == val); }

extern const nil_type nil;

}

#endif // ELBA_NIL_HPP
