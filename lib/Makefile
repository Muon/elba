TOP_DIR = ..

include $(TOP_DIR)/Makefile.defaults

VPATH = $(TOP_DIR)/$(INC_DIR) $(TOP_DIR)/$(SRC_DIR)

ELBA_SRC = elba_state.cpp elba_stack.cpp elba_table.cpp elba_reference.cpp
ELBA_O = $(ELBA_SRC:%.cpp=%.o)
ELBA_A = libelba.a
ELBA_SO = libelba.so
LIBS = ../liblua.a -ldl

lib: shared static

shared: $(ELBA_SO)

static: $(ELBA_A)

$(ELBA_SO): $(ELBA_O)
	$(CXX) -shared $(ELBA_O) $(LIBS) -o $@

$(ELBA_A): $(ELBA_A)($(ELBA_O))
	$(RANLIB) $@

clean:
	$(RM) $(ELBA_SO) $(ELBA_A) $(ELBA_O) depend

depend: $(ELBA_SRC)
	@$(ECHO) -n "Building dependencies... "
	@> depend
	@$(foreach FILE,$(ELBA_SRC), $(CPP) $(CPPFLAGS) -MM $(TOP_DIR)/$(SRC_DIR)/$(FILE) >> depend;)
	@$(ECHO) "done."

include depend

.PHONY: lib shared static clean