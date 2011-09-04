#ifndef ELBA_NIL_HPP
#define ELBA_NIL_HPP

namespace elba
{

class reference;

struct nil_type
{
	nil_type() {}

	operator bool() const { return false; }

	bool operator==(const nil_type& n) const { return true; }
};

bool operator==(const nil_type& n, const reference& ref);
template<typename T> bool operator==(const nil_type& n, const T& val) { return false; }
template<typename T> bool operator!=(const nil_type& n, const T& val) { return !(n == val); }

extern const nil_type nil;

}

#endif // ELBA_NIL_HPP