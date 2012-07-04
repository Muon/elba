TOP_DIR = ..

include $(TOP_DIR)/Makefile.defaults

VPATH = $(TOP_DIR)/$(INC_DIR) $(TOP_DIR)/$(TEST_DIR)

ELBA_A = $(TOP_DIR)/$(LIB_DIR)/libelba.a

LIBS = $(ELBA_A) -llua -lgtest -lgtest_main

TESTS_SRC = state_test.cpp reference_test.cpp stack_test.cpp
TESTS_O = $(TESTS_SRC:%.cpp=%.o)
TESTS_BIN = elba_test_suite

test: $(TESTS_BIN)
	@./$(TESTS_BIN)

$(TESTS_BIN): $(ELBA_A) $(TESTS_O)
	$(CXX) $(TESTS_O) $(LIBS) -o $@

$(ELBA_A):
	$(MAKE) -C $(TOP_DIR)/$(LIB_DIR)

clean:
	$(RM) $(TESTS_BIN) $(TESTS_O) depend

depend: $(TESTS_SRC)
	@$(ECHO) -n "Building dependencies... "
	@> depend
	@$(foreach FILE,$(TESTS_SRC), $(CPP) $(CPPFLAGS) -MM $(TOP_DIR)/$(TEST_DIR)/$(FILE) >> depend;)
	@$(ECHO) "done."

include depend

.PHONY: test clean
