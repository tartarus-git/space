MAIN_CPP_INCLUDES              := src/debug/logger.h
DEBUG_LOGGER_CPP_INCLUDES      := src/debug/logger.h
EXIT_PROGRAM_CPP_INCLUDES      := src/exit_program.h
MAIN_GAME_CODE_CPP_INCLUDES    := src/main_game_code.h
CAMERA_T_CPP_INCLUDES          := src/rendering/camera_t.h
ENTITY_RENDERER_T_CPP_INCLUDES := src/rendering/entity_renderer_t.h
ENTITY_SHADER_T_CPP_INCLUDES   := src/shaders/entity_shader_t.h
SHADER_T_CPP_INCLUDES          := src/shaders/shader_t.h
IMPLEMENTATION_CPP_INCLUDES    := src/debug/logger.h src/exit_program.h

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

CLANG_PREAMBLE := $(CLANG_PROGRAM_NAME) -std=$(CPP_STD) -$(OPTIMIZATION_LEVEL) $(POSSIBLE_WALL) -fno-exceptions -pthread

.PHONY: all unoptimized touch_all_necessary clean clean_include_swaps

all: bin/$(BINARY_NAME)

unoptimized:
	$(MAKE) OPTIMIZATION_LEVEL:=O0

bin/$(BINARY_NAME): bin/main.o bin/debug/logger.o bin/exit_program.o bin/main_game_code.o bin/rendering/camera_t.o bin/rendering/entity_renderer_t.o bin/rendering/renderer_t.o bin/shaders/entity_shader_t.o bin/shaders/shader_t.o
	$(CLANG_PREAMBLE) -o bin/$(BINARY_NAME) bin/main.o bin/debug/logger.o -lGL -lglfw

bin/main.o: src/main.cpp $(MAIN_CPP_INCLUDES) bin/.dirstamp
	$(CLANG_PREAMBLE) -c -Isrc -o bin/main.o src/main.cpp

bin/debug/logger.o: src/debug/logger.cpp $(DEBUG_LOGGER_CPP_INCLUDES) bin/debug/.dirstamp
	$(CLANG_PREAMBLE) -c -Isrc -o bin/debug/logger.o src/debug/logger.cpp

bin/exit_program.o: src/exit_program.cpp $(EXIT_PROGRAM_CPP_INCLUDES) bin/.dirstamp
	$(CLANG_PREAMBLE) -c -Isrc -o bin/exit_program.o src/exit_program.cpp

bin/main_game_code.o: src/main_game_code.cpp $(MAIN_GAME_CODE_CPP_INCLUDES) bin/.dirstamp
	$(CLANG_PREAMBLE) -c -Isrc -o bin/main_game_code.o src/main_game_code.cpp

bin/rendering/camera_t.o: src/rendering/camera_t.cpp $(CAMERA_T_CPP_INCLUDES) bin/rendering/.dirstamp
	$(CLANG_PREAMBLE) -c -Isrc -o bin/rendering/camera_t.o src/rendering/camera_t.cpp

bin/rendering/entity_renderer_t.o: src/rendering/entity_renderer_t.cpp $(ENTITY_RENDERER_T_CPP_INCLUDES) bin/rendering/.dirstamp
	$(CLANG_PREAMBLE) -c -Isrc -o bin/rendering/entity_renderer_t.o src/rendering/entity_renderer_t.cpp

bin/rendering/renderer_t.o: src/rendering/renderer_t.cpp $(RENDERER_T_CPP_INCLUDES) bin/rendering/.dirstamp
	$(CLANG_PREAMBLE) -c -Isrc -o bin/rendering/renderer_t.o src/rendering/renderer_t.cpp

bin/shaders/entity_shader_t.o: src/shaders/entity_shader_t.cpp $(ENTITY_SHADER_T_CPP_INCLUDES) bin/shaders/.dirstamp
	$(CLANG_PREAMBLE) -c -Isrc -o bin/shaders/entity_shader_t.o src/shaders/entity_shader_t.cpp

bin/shaders/shader_t.o: src/shaders/shader_t.cpp $(SHADER_T_CPP_INCLUDES) bin/shaders/.dirstamp
	$(CLANG_PREAMBLE) -c -Isrc -o bin/shaders/shader_t.o src/shaders/shader_t.cpp

bin/opengl/implementation.o: src/opengl/implementation.cpp $(IMPLEMENTATION_CPP_INCLUDES) bin/opengl/.dirstamp
	$(CLANG_PREAMBLE) -c -Isrc -o bin/opengl/implementation.o src/opengl/implementation.cpp

bin/.dirstamp:
	mkdir -p bin
	touch bin/.dirstamp

bin/debug/.dirstamp: bin/.dirstamp
	mkdir -p bin/debug
	touch bin/debug/.dirstamp

bin/rendering/.dirstamp: bin/.dirstamp
	mkdir -p bin/rendering
	touch bin/rendering/.dirstamp

bin/shaders/.dirstamp: bin/.dirstamp
	mkdir -p bin/shaders
	touch bin/shaders/.dirstamp

bin/opengl/.dirstamp: bin/.dirstamp
	mkdir -p bin/opengl
	touch bin/opengl/.dirstamp

touch_all_necessary:
	touch src/main.cpp
	touch src/debug/logger.cpp
	touch src/exit_program.cpp
	touch src/main_game_code.cpp
	touch src/rendering/camera_t.cpp
	touch src/rendering/entity_renderer_t.cpp
	touch src/rendering/renderer_t.cpp
	touch src/shaders/entity_shader_t.cpp
	touch src/shaders/shader_t.cpp
	touch src/opengl/implementation.cpp

# The normal clean rule ignores swap files in case you have open vim instances. This is respectful to them.
# Use the clean_include_swaps rule to clean every untracked file. You can do that if you don't have any vim instances open.
clean:
	git clean -fdx -e *.swp

clean_include_swaps:
	git clean -fdx
