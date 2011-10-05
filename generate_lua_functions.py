#!/usr/bin/env python

import sys

def usage():
	print("""Usage:	generate_lua_functions.py filename [n]
	filename	name of file in which to insert the code
	n		number of function parameters to support

Code is inserted between the markers \"// START LUA FUNC GLUE\" and
\"// END LUA FUNC GLUE\" (sans quotes), replacing any text or code
between them.""")

if len(sys.argv) == 1:
	usage()
	sys.exit()

total = sys.argv[2] if len(sys.argv) == 3 else 8

functions = list()
for n in range(1, total + 1):
	typenames = list('T{0}'.format(i) for i in range(1, n + 1))

	functions.append("""	template<{}>
	reference operator()({})
	{{
		stack st(L);
		st.push(*this);

{}

		st.call({}, 1);

		reference ref(L, stack::top);

		st.pop(1);

		return ref;
	}}""".format(
		", ".join("typename T{}".format(i) for i in range(1, n + 1)),
		", ".join("const T{0}& arg{0}".format(i) for i in range(1, n + 1)),
		"\n".join("		st.push(arg{});".format(i) for i in range(1, n+1)),
		n
	))

f = open(sys.argv[1], "r")
header = f.read()
f.close()

start_marker = "// START LUA FUNC GLUE"
end_marker = "// END LUA FUNC GLUE"

start_index = header.index(start_marker)
end_index = header.index(end_marker, start_index)

header = header[:start_index + len(start_marker)] + "\n" + \
	"\n\n".join(functions) + "\n" + header[end_index:]

f = open(sys.argv[1], "w")
f.write(header)
f.close()