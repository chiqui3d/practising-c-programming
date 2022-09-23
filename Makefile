PROJDIR = $(realpath $(CURDIR))
INC_DIR = include

CC :=clang
CFLAGS := -std=c17 -D_XOPEN_SOURCE=700 -D_GNU_SOURCE=1 -MMD -Wall -Wextra -Wno-vla -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow
CFLAGS += -fsanitize=signed-integer-overflow -fsanitize=undefined
CFLAGS += -ggdb3
#CFLAGS += -lcrypt -lcs50
CFLAGS += $(shell pkg-config --cflags json-c)
LDFLAGS += $(shell pkg-config --libs json-c)
#CFLAGS += -DNDEBUG # Not debug h_debug.h o disable asserts

# form command=> `pkg-config --cflags --libs glib-2.0`
CFLAGS += -I$(INC_DIR) -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -lglib-2.0
CFLAGS += -I/usr/local/include
CFLAGS += -L/usr/local/lib -lcrypto
CFLAGS += -DE4C_THREADSAFE=1
#CFLAGS += -D_OPENSSL_API_COMPAT=0x10000000L 
LDFLAGS += -L/usr/local/ssl/lib
LDLIBS += -lssl -lcrypto -lpam -lpam_misc
LDLIBS += -pthread

# https://stackoverflow.com/questions/5593284/undefined-reference-to-ssl-library-init-and-ssl-load-error-strings

# http parser
CFLAGS += -I$(PROJDIR)/include/llhttp/build

# userspace_cooperative_multitasking
CFLAGS += -I$(PROJDIR)/threads/userspace_cooperative_multitasking/include

# -L$(PROJDIR)/include/llhttp/build -lllhttp
#$(PROJDIR)/include/llhttp/build/libllhttp.a


SRCS = $(wildcard *.c) $(wildcard **/*.c) $(wildcard **/**/*.c) # all c files
OBJ = $(SRCS:.c=.o) # all objects files
DEP = $(OBJ:.o=.d)  # one dependency file for each source	
BINS = $(SRCS:%.c=%) # all files bin files
INCLUDES_FILES = $(wildcard include/*.c) $(wildcard threads/userspace_cooperative_multitasking/include/*.c)
INCLUDES_OBJECTS = $(INCLUDES_FILES:.c=.o)
INCLUDES_HTTP =  $(PROJDIR)/include/llhttp/build/c/llhttp.o $(PROJDIR)/include/llhttp/build/native/api.o $(PROJDIR)/include/llhttp/build/native/http.o

# -pg gprof profile generator gmon.out and then gprof program > result.tx gprof
# -v Show commands to run and use verbose output
# -D Definitions for some extra functions that are defined in the X/Open and POSIX standards
# -D_XOPEN_SOURCE=200112L ftruncate()
# -I <directorio> Adds a path for headers search
# -MMD generate dependency .d files
# -g specifies that we want to include "debugging symbols" which allows us to use a debugging program.
# -Wall turns on most, but not all, compiler warnings
# -o <output file> specifies the name of the output file
# -c Execute all of the above, plus the assembler, generating a target ".o" object file.
# -W<aviso> Activates a specific warning. With -Weverything you activate all diagnostics. With -Werror, warnings are transformed into errors.
# -Wno-vla turns off the warning about variable-length arrays.
# -O0 specifies to do no optimizations on our code.
# -O2 obtimization level 2
# -stdlib=libc++ specifies that we want to use the standard library implementation called libc++
# -std=c++1y means we want to use the C++14 standard (called 1y in this version of Clang).
# -l linked library name -lssl -lcrypto -lcs50. (PAM) library is called libpam.a, To link in 
# libpam.a, you use a command like this: -lpam
# -L. search libraries in the current directory 

# -fPIC test1 => test1.o generate A shared library (also known as a shared object, or as a dynamically linked library)
# share library generate file with extension .so
# gcc -shared -fPIC -o libtest.so test1.o test2.o
# gcc -o app app.o -L. –ltest witout prefix lib and so extension
# gcc -o app app.o -L. -ltest -Wl,-rpath,/usr/local/lib or use LD_LIBRARY_PATH
# ldd app: show the dependencies of the program
# ar cr libtest.a test1.o test2.o: create static library archive file


# Examples from other makefiles: 
# -I /usr/include/glib‑2.0 ‑I /usr/lib/glib‑2.0/include ‑lglib‑2.0
# CFLAGS=-fsanitize=signed-integer-overflow -fsanitize=undefined -ggdb3 -O0 -std=c11 -Wall -Werror -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow
# -I$(INC_DIR)/.. https://stackoverflow.com/questions/22545029/makefile-how-to-correctly-include-header-file-and-its-directory

.PHONY: all clean test includes
test:
	@echo "Project directory: $(PROJDIR)"
	@echo "Objects files:"
	@echo $(OBJ)
	@echo "Code files:"
	@echo $(SRCS)
	@echo "Binary files:"
	@echo $(BINS)

all: $(OBJ)
includes: $(INCLUDES_OBJECTS)

# $(PROJDIR)/include/llhttp/build/libllhttp.a
# target: dependencies
# macro $@:$?
# macros: $? $@ $* $ $<
%: %.o includes
	@echo "Compiling..."
	$(CC) $(LDLIBS) $(CFLAGS) $(LDFLAGS) $(INCLUDES_OBJECTS) $(INCLUDES_HTTP)  $@.o -o $@
	@echo "Run program..."
	@./$@

%.o: %.c
	@echo "Created object..."
	$(CC) $(LDLIBS) $(LDFLAGS) $(CFLAGS) -c $< -o $@

#vpath %.o $(INCLUDES_OBJECTS)
#-include $(INCLUDES_OBJECTS)   # include all dep files in the makefile


# rule to generate a dependency (.h) file by using the C preprocessor
# (see man cpp for details on the -MM and -MT options)
# %.d: %.c
# 	@echo "compiling ...2"
# 	@$(CC) $(LDLIBS) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@
# 	@echo "Run ...2"

clean:
	@echo "Cleaning bin..."
	rm -f ${BINS}
	@echo "Cleaning objects..."
	rm -f ${OBJ}
	@echo "Cleaning deps..."
	rm -f ${DEP}

.PHONY: errors1 count scanf3

errors1: errors/errors1.c errors/e4c.c
	$(CC) $(LDLIBS) $(CFLAGS) -o errors/errors1 errors/errors1.c errors/e4c.c

scope: learn-hard-way/scope/main.o learn-hard-way/scope/ex22.o
	$(CC) $(LDLIBS) $(CFLAGS) -o learn-hard-way/scope/main learn-hard-way/scope/main.c learn-hard-way/scope/ex22.o

count: pointers/count.c
	$(CC) $(LDLIBS) $(CFLAGS) -o pointers/count pointers/count.c

scanf3: scanf3.c get_line.c
	$(CC) $(LDLIBS) $(CFLAGS) -o scanf3 scanf3.c get_line.c

.PHONY: functions glib rocket linked_list

functions: functions.c
	$(CC) $(LDLIBS) $(CFLAGS) -o functions functions.c

glib: glib.c
	$(CC) $(LDLIBS) $(CFLAGS) -o glib glib.c

linked_list: data-structures/me/linked_list.c
	$(CC) $(LDLIBS) $(CFLAGS) -o  data-structures/me/linked_list  data-structures/me/linked_list.c
	
# https://github.com/vivek9236/rocket_game/blob/master/Makefile
rocket: sdl/rocket.c
	$(CC) $(LDLIBS) $(CFLAGS) `sdl2-config --libs --cflags` -lSDL2_image $(shell sdl2-config --cflags) -o sdl/rocket sdl/rocket.c

PREFIX_INSTALL = /usr/local

# http://nuclear.mutantstargoat.com/articles/make/#installing-libraries
.PHONY: install
install: data-structures/me/linked_list
	mkdir -p $(DESTDIR)$(PREFIX_INSTALL)/bin
	cp $< $(DESTDIR)$(PREFIX_INSTALL)/bin/linked_list

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX_INSTALL)/bin/linked_list