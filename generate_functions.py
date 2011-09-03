#!/usr/bin/env python

import sys

def usage():
	print("""Usage:	generate_functions.py filename [n]
	filename	name of file in which to insert the code
	n		number of function parameters to support

Code is inserted between the markers \"// START FREE FUNC GLUE\" and
\"// END FREE FUNC GLUE\" (sans quotes), replacing any text or code
between them.""")

if len(sys.argv) == 1:
	usage()
	sys.exit()

total = sys.argv[2] if len(sys.argv) == 3 else 8

functions = list()

for n in range(1, total + 1):
	typenames = list('T{0}'.format(i) for i in range(1, n + 1))
	def funcptr_signature(name, void_return=False):
		return "{} (*{})({})".format("void" if void_return else "R", name, ", ".join(typenames))

	functions.append("""	template<typename R, {}>
	void push({}) const
	{{
		struct wrapper_creator
		{{
			typedef {};

			static int wrapper(lua_State* L)
			{{
				stack st(L);

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get<bindable_funcptr>(st.upvalue_index(1)));

				st.push(function_real({}));

				return 1;
			}}
		}};

		push(reinterpret_cast<bindable_funcptr>(func_ptr));
		push(wrapper_creator::wrapper, 1);
	}}""".format(
		", ".join("typename {}".format(T) for T in typenames),
		funcptr_signature("func_ptr"),
		funcptr_signature("wrapped_funcptr"),
		", ".join("st.get<T{0}>({0})".format(i) for i in range(1, n + 1))
	))

	functions.append("""	template<{}>
	void push({}) const
	{{
		struct wrapper_creator
		{{
			typedef {};

			static int wrapper(lua_State* L)
			{{
				stack st(L);

				wrapped_funcptr function_real = reinterpret_cast<wrapped_funcptr>(st.get<bindable_funcptr>(st.upvalue_index(1)));

				function_real({});

				return 0;
			}}
		}};

		push(reinterpret_cast<bindable_funcptr>(func_ptr));
		push(wrapper_creator::wrapper, 1);
	}}""".format(
		", ".join("typename {}".format(T) for T in typenames),
		funcptr_signature("func_ptr", True),
		funcptr_signature("wrapped_funcptr", True),
		", ".join("st.get<T{0}>({0})".format(i) for i in range(1, n + 1))
	))

f = open(sys.argv[1], "r")
header = f.read()
f.close()

start_marker = "// START FREE FUNC GLUE"
end_marker = "// END FREE FUNC GLUE"

start_index = header.index(start_marker)
end_index = header.index(end_marker, start_index)

header = header[:start_index + len(start_marker)] + "\n" + \
	"\n\n".join(functions) + "\n" + header[end_index:]

f = open(sys.argv[1], "w")
f.write(header)
f.close()
