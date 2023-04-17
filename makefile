undefine MAIN_CPP_INCLUDES

BINARY_NAME := space

CLANG_PROGRAM_NAME := clang++-15
CPP_STD := c++20
OPTIMIZATION_LEVEL := O3
USE_WALL := true
ifeq ($(USE_WALL), true)
POSSIBLE_WALL := -Wall
else
undefine POSSIBLE_WALL
endif

CLANG_PREAMBLE := $(CLANG_PROGRAM_NAME) -std=$(CPP_STD) -$(OPTIMIZATION_LEVEL) $(POSSIBLE_WALL) -fno-exceptions

.PHONY: all unoptimized touch_all_necessary clean clean_include_swaps

all: bin/$(BINARY_NAME)

unoptimized:
	$(MAKE) OPTIMIZATION_LEVEL:=O0

bin/$(BINARY_NAME): bin/main.o
	$(CLANG_PREAMBLE) -o bin/$(BINARY_NAME) bin/main.o

bin/main.o: src/main.cpp $(MAIN_CPP_INCLUDES) bin/.dirstamp
	$(CLANG_PREAMBLE) -c -Isrc -o bin/main.o src/main.cpp

bin/.dirstamp:
	mkdir -p bin
	touch bin/.dirstamp

touch_all_necessary:
	touch src/main.cpp

# The normal clean rule ignores swap files in case you have open vim instances. This is respectful to them.
# Use the clean_include_swaps rule to clean every untracked file. You can do that if you don't have any vim instances open.
clean:
	git clean -fdx -e *.swp

clean_include_swaps:
	git clean -fdx
