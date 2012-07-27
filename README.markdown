# Elba

[![Build Status](https://secure.travis-ci.org/Muon/elba.png)](http://travis-ci.org/Muon/elba)

Elba is a simple-to-use library designed to enable the easy bridging of the
chasm between the worlds of C++ and Lua 5.1. At the moment, Elba is incomplete
and lacks documentation, so it is not recommended for production usage.

## Example

### Simple `elba::reference`-based manipulation

```c++
#include <elba.hpp>
#include <iostream>

void report(const char* str)
{
	std::cout << str << std::endl;
}

int main()
{
	elba::state L;
	L.open_libs();

	elba::reference print = L.globals()["print"];
	print("C++ to Lua, C++ to Lua, over *kzzzht*");

	L.globals()["report"] = report;
	L.do_string("report(\"Lua to C++, report! *kzzzht*\")");

	return 0;
}
```

### Binding classes

```c++
#include <elba.hpp>

class Counter
{
public:
	Counter() : counter(0) {}

	void increment() { ++counter; }
	void decrement() { --counter; }
	int count() const { return counter; }

private:
	int counter;
};

int main()
{
	elba::state L;
	L.open_libs();

	elba::class_binder<Counter>(L, "Counter")
		.constructor()
		.set("increment", &Counter::increment)
		.set("decrement", &Counter::decrement)
		.set("count", &Counter::count)
		.set("primary_counter", Counter());

	L.do_string(
		"a = Counter.new() \
		print(a:count()) \
		a:increment() \
		print(a:count()) \
		print(Counter.primary_counter:count())"
	);

	return 0;
}
```

## Features

* `operator[]`-style table manipulation
* primitive type and std::string assignment/retrieval
* binding functions
* retrieval of functions from the Lua state
* class binding

## Todo

* class properties
* table iterators
* review code
* write documentation

## License

Copyright (c) 2011-2012 Mak Nazečić-Andrlon

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
