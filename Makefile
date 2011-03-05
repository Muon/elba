all: lib

lib:
	$(MAKE) -C lib

test:
	$(MAKE) -C bin

clean:
	$(RM) lib/*.o lib/*.a  bin/*_test

.PHONY: all lib test