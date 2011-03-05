VPATH = ../tests
LIBS = -L../lib -lelba -llua
INCLUDES = -I../include
TESTS = print_test
ECHO = echo -e

test: $(TESTS)

%_test: %_test.cpp
	@$(ECHO) -------"\n"$@"\n"-------
	@($(CXX) $(INCLUDES) $(CXXFLAGS) $< $(LIBS) -o $@ && ./$@ && $(ECHO) -------"\n"passed) || ($(ECHO) -------"\n"failed && false)

.PHONY: tests
