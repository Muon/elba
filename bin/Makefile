TOP_DIR = ..

include $(TOP_DIR)/Makefile.defaults

VPATH = $(TOP_DIR)/$(INC_DIR) $(TOP_DIR)/$(TEST_DIR)

CXXFLAGS += -L$(TOP_DIR)/$(LIB_DIR)

#LIBS = -lelba
LIBS = $(TOP_DIR)/$(LIB_DIR)/libelba.a -llua

TESTS_SRC = print_test.cpp set_test.cpp free_function_test.cpp class_binding_test.cpp nil_test.cpp

TESTS =  $(TESTS_SRC:%.cpp=%)

test: $(TESTS)
	@$(foreach TEST,$(TESTS),\
		$(ECHO) $(TEST)\ ; \
		((LD_LIBRARY_PATH=$(TOP_DIR)/$(LIB_DIR) ./$(TEST)) \
		&& $(ECHO) passed) \
		|| ($(ECHO) failed && false);)

$(TESTS): $(TOP_DIR)/$(LIB_DIR)/libelba.a

$(TOP_DIR)/$(LIB_DIR)/libelba.a:
	$(MAKE) -C $(TOP_DIR)/$(LIB_DIR)

%_test: %_test.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< $(LIBS) -o $@

clean:
	$(RM) $(TESTS) depend

depend: $(TESTS_SRC)
	@$(ECHO) -n "Building dependencies... "
	@> depend
	@$(foreach FILE,$(TESTS_SRC), $(CPP) $(CPPFLAGS) -MM $(TOP_DIR)/$(TEST_DIR)/$(FILE) >> depend;)
	@$(ECHO) "done."

include depend

.PHONY: test clean