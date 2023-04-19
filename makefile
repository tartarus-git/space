# TODO: Create some nice command-line tools to automate some of this.
# For example, there should be a tool to automatically generate the user header files that cpp files include so we don't need
# to keep the following list up-to-date.
# There should also be a tool to list all the cpp files in a folder tree and write them side-by-side for use in the makefile.
# Maybe also a tool to generate the bin/debug/x.o: etc... pattern over and over for every file.

MAIN_CPP_INCLUDES              := src/main_game_code.cpp src/debug/logger.h src/exit_program.h
DEBUG_LOGGER_CPP_INCLUDES      := src/debug/logger.h
EXIT_PROGRAM_CPP_INCLUDES      := src/exit_program.h
MAIN_GAME_CODE_CPP_INCLUDES    := src/main_game_code.h src/geometry/mesh_t.h src/texturing/texture_t.h src/entities/entity_t.h src/scene/scene_t.h src/rendering/camera_t.h src/math/matrix4f_t.h src/rendering/entity_renderer_t.h src/debug/logger.h src/exit_program.h
CAMERA_T_CPP_INCLUDES          := src/rendering/camera_t.h src/math/matrix4f_t.h
ENTITY_RENDERER_T_CPP_INCLUDES := src/rendering/renderer_t.h src/rendering/entity_renderer_t.h src/scene/scene_t.h
RENDERER_T_CPP_INCLUDES        := src/rendering/renderer_t.h src/scene/scene_t.h
ENTITY_SHADER_T_CPP_INCLUDES   := src/shaders/entity_shader_t.h src/math/matrix4f_t.h
SHADER_T_CPP_INCLUDES          := src/shaders/shader_t.h src/math/matrix4f_t.h src/debug/logger.h src/exit_program.h
IMPLEMENTATION_CPP_INCLUDES    := src/debug/logger.h src/exit_program.h
MESH_T_CPP_INCLUDES            := src/geometry/mesh_t.h src/math/vector3f_t.h src/math/vector3ui_t.h

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

bin/$(BINARY_NAME): bin/main.o bin/debug/logger.o bin/exit_program.o bin/main_game_code.o bin/rendering/camera_t.o bin/rendering/entity_renderer_t.o bin/rendering/renderer_t.o bin/shaders/entity_shader_t.o bin/shaders/shader_t.o bin/opengl/implementation.o bin/geometry/mesh_t.o
	$(CLANG_PREAMBLE) -o bin/$(BINARY_NAME) bin/main.o bin/debug/logger.o bin/exit_program.o bin/main_game_code.o bin/rendering/camera_t.o bin/rendering/entity_renderer_t.o bin/rendering/renderer_t.o bin/shaders/entity_shader_t.o bin/shaders/shader_t.o bin/opengl/implementation.o bin/geometry/mesh_t.o -lGL -lglfw

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

bin/geometry/mesh_t.o: src/geometry/mesh_t.cpp $(MESH_T_CPP_INCLUDES) bin/geometry/.dirstamp
	$(CLANG_PREAMBLE) -c -Isrc -o bin/geometry/mesh_t.o src/geometry/mesh_t.cpp

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

bin/geometry/.dirstamp: bin/.dirstamp
	mkdir -p bin/geometry
	touch bin/geometry/.dirstamp

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
	touch src/geometry/mesh_t.cpp

# The normal clean rule ignores swap files in case you have open vim instances. This is respectful to them.
# Use the clean_include_swaps rule to clean every untracked file. You can do that if you don't have any vim instances open.
clean:
	git clean -fdx -e *.swp

clean_include_swaps:
	git clean -fdx
