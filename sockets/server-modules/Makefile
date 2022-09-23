########################################################################
# Code listing from "Advanced Linux Programming," by CodeSourcery LLC  #
# Copyright (C) 2001 by New Riders Publishing                          #
# See COPYRIGHT for license information.                               #
########################################################################

### Configuration.  ####################################################

PROJDIR = $(realpath $(CURDIR))
CC 	    :=clang
CFLAGS  := -std=c17 -D_XOPEN_SOURCE=700 -D_GNU_SOURCE=1 
CFLAGS  := -MMD -Wall -Wextra -Wno-vla -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow
CFLAGS  += -fsanitize=signed-integer-overflow -fsanitize=undefined


# C source files for the server.
SOURCES		=  $(wildcard *.c)
# Object files corresponding to source files.
OBJECTS		= $(SOURCES:.c=.o)
# Server module shared library files. 
MODULES_C	= $(wildcard modules/*.c)
MODULES		= $(MODULES_C:.c=.so)

DEP 		= $(wildcard *.d) $(wildcard modules/*.d)
DEPS 		= server.h

### Rules.  ############################################################

.PHONY: all clean test

test:
	@echo "Project directory: $(PROJDIR)"
	@echo "SOURCES files:"
	@echo $(SOURCES)
	@echo "MODULES files:"
	@echo $(MODULES)

# Default target: build everything.
all: server $(MODULES)

# Clean up build products.
clean:
	rm -f $(OBJECTS) $(MODULES) $(DEP) server

# The main server program.  Link with -Wl,-export-dyanamic so
# dynamically loaded modules can bind symbols in the program.  Link in
# libdl, which contains calls for dynamic loading.
server: $(OBJECTS)
	$(CC) $(CFLAGS) -Wl,-export-dynamic -o $@ $^ -ldl

# All object files in the server depend on server.h.  But use the
# default rule for building object files from source files.
$(OBJECTS): $(DEPS)

# Rule for building module shared libraries from the corresponding
# source files.Compile -fPIC and generate a shared object file.
$(MODULES): \
%.so: %.c $(DEPS)
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $<
