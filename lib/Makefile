VPATH = ../include ../src

CXXFLAGS = -g
LIBS = -llua
ELBA_O = elba_interpreter.o
ELBA_A = libelba.a

$(ELBA_A): $(ELBA_O)
	$(AR) rcu $(ELBA_A) $(ELBA_O)

elba_interpreter.o : elba_interpreter.hpp
