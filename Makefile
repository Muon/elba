TOP_DIR = .

include Makefile.defaults

all: lib

lib:
	$(MAKE) -C lib

test: lib
	$(MAKE) -C bin

clean:
	$(MAKE) -C lib clean
	$(MAKE) -C bin clean

.PHONY: all lib test clean