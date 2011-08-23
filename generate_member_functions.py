#!/usr/bin/env python

import sys

def usage():
	print("""Usage:	generate_member_functions.py filename [n]
	filename	name of file in which to insert the code
	n		number of function parameters to support

Code is inserted between the markers \"// START MEM FUNC GLUE\" and
\"// END MEM FUNC GLUE\" (sans quotes), replacing any text or code
between them.""")

if len(sys.argv) == 1:
	usage()
	sys.exit()

total = sys.argv[2] if len(sys.argv) == 3 else 8

functions = list()

for n in range(1, total + 1):
	typenames = list('T{0}'.format(i) for i in range(1, n + 1))
	def funcptr_signature(name, void_return=False, const=False):
		return "{} (C::*{})({}) {}".format("void" if void_return else "R", name, ", ".join(typenames), "const" if const else "")

	def add_pusher(is_const, returns_void):
		functions.append("""	template<typename R, typename C, {}>
	void push({}) const
	{{
		struct wrapper_creator
		{{
			typedef {};

			static int wrapper(lua_State* L)
			{{
				stack st(L);

				memfunptr* func_ptrptr;
				st.get(func_ptrptr, st.upvalue_index(1));

				C* object = NULL;
				st.get(object, 1);
{}

				memfunptr func = *func_ptrptr;

				st.push((object->*func)({}));

				return 1;
			}}
		}};

		push_memfun_wrapper(wrapper_creator::wrapper, func_ptr);
	}}""".format(
			", ".join("typename {}".format(T) for T in typenames),
			funcptr_signature("func_ptr", returns_void, is_const),
			funcptr_signature("memfunptr", returns_void, is_const),
			"\n".join("""				T{0} arg{0};
				st.get(arg{0}, {1});""".format(i, i + 1) for i in range(1, n + 1)),
			", ".join("arg{}".format(i) for i in range(1, n + 1))
		))

	add_pusher(False, False)
	add_pusher(True, False)
	add_pusher(False, True)
	add_pusher(True, True)

f = open(sys.argv[1], "r")
header = f.read()
f.close()

start_marker = "// START MEM FUNC GLUE"
end_marker = "// END MEM FUNC GLUE"

start_index = header.index(start_marker)
end_index = header.index(end_marker, start_index)

header = header[:start_index + len(start_marker)] + "\n" + \
	"\n\n".join(functions) + "\n" + header[end_index:]

f = open(sys.argv[1], "w")
f.write(header)
f.close()
