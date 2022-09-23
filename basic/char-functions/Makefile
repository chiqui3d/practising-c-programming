### References ###

# * https://github.com/JackWetherell/c-project-structure/blob/master/Makefile
# * https://stackoverflow.com/questions/5178125/how-to-place-object-files-in-separate-subdirectory


### Configuration ###

PROJDIR  := $(realpath $(CURDIR))
SRCDIR   := src
TARGET 	 := char-functions

CC 	     := clang
CFLAGS   := -std=c17 -D_XOPEN_SOURCE=700 -D_GNU_SOURCE=1 
CFLAGS   += -MMD -Wall -Wextra -Wno-vla -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow
CFLAGS   += -fsanitize=signed-integer-overflow -fsanitize=undefined


# include headers files from include directory
CFLAGS   += -I include

# C source files for the main program.
SOURCES			:= $(wildcard $(PROJDIR)/*.c)
# Object files corresponding to source files.
OBJECTS     	:= $(SOURCES:.c=.o)


INCLUDES_FILES = $(wildcard include/*.c)
INCLUDES_OBJECTS = $(INCLUDES_FILES:.c=.o)

# Dependency files corresponding to object files.
DEP 			:= $(OBJECTS:.o=.d) $(INCLUDES_OBJECTS:.o=.d)

### Rules ###

.PHONY: all char-functions includes

all: $(TARGET)

# The main server program
char-functions: $(OBJECTS) $(INCLUDES_OBJECTS)
	@echo "Compiling..."
	$(CC) $(CFLAGS) $^ -o $(TARGET)
	@echo "Run..."
	@./$(TARGET)

# Build object files from C source files.
%.o:%.c FORCE
	@echo "Created object $@ ..."
	$(CC) $(CFLAGS) -c $< -o $@

# Build object files from C source lib files.
# $(LIBRARY_OBJECTS):$(LIBRARY_SOURCES) FORCE
# 	@echo "Created object $@ ..."
# 	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: FORCE test clean
FORCE:

test:
	@echo "Project directory: $(PROJDIR)"
	@echo "SOURCES:"
	@echo $(SOURCES)
	@echo "OBJECTS:"
	@echo $(OBJECTS)

# Clean up
clean:
	rm -f $(OBJECTS) $(INCLUDES_OBJECTS) $(DEP) $(TARGET)