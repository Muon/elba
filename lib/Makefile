TOP_DIR = ..

include $(TOP_DIR)/Makefile.defaults

VPATH = $(TOP_DIR)/$(INC_DIR) $(TOP_DIR)/$(SRC_DIR)

ELBA_SRC = elba_state.cpp elba_stack.cpp elba_table.cpp
ELBA_O = $(ELBA_SRC:%.cpp=%.o)
ELBA_A = libelba.a
ELBA_SO = libelba.so
LIBS = -llua

lib: shared static

shared: $(ELBA_SO)

static: $(ELBA_A)

$(ELBA_SO): $(ELBA_O)
	$(CXX) -shared $(LIBS) $(ELBA_O) -o $@

$(ELBA_A): $(ELBA_A)($(ELBA_O))
	$(RANLIB) $@

clean:
	$(RM) $(ELBA_SO) $(ELBA_A) $(ELBA_O) depend

depend: $(ELBA_SRC)
	$(RM) depend
	$(foreach FILE,$(ELBA_SRC), $(CPP) $(CPPFLAGS) -MM $(TOP_DIR)/$(SRC_DIR)/$(FILE) >> depend;)

include depend

.PHONY: lib shared static clean