TOP_DIR = ..

include $(TOP_DIR)/Makefile.defaults

VPATH = $(TOP_DIR)/$(INC_DIR) $(TOP_DIR)/$(SRC_DIR)

ELBA_SRC = elba_state.cpp elba_stack.cpp elba_reference.cpp elba_nil.cpp elba_error.cpp elba_activation_record.cpp
ELBA_O = $(ELBA_SRC:%.cpp=%.o)
ELBA_A = libelba.a
ELBA_SO = libelba.so
LIBS = $(LUA_LIBS)
CXXFLAGS += $(LUA_CXXFLAGS)
CPPFLAGS += $(LUA_CPPFLAGS)

ifeq ($(shell uname -m),x86_64)
  CXXFLAGS += -fPIC
endif

lib: shared static

shared: $(ELBA_SO)

static: $(ELBA_A)

$(ELBA_SO): $(ELBA_O)
	$(CXX) $(LDFLAGS) -shared $(ELBA_O) $(LIBS) -o $@

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
