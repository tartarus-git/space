# TODO: Create some nice command-line tools to automate some of this.
# For example, there should be a tool to automatically generate the user header files that cpp files include so we don't need
# to keep the following list up-to-date.
# There should also be a tool to list all the cpp files in a folder tree and write them side-by-side for use in the makefile.
# Maybe also a tool to generate the bin/debug/x.o: etc... pattern over and over for every file.

# TODO: You definitely need to create a tool to get all headers because headers in headers don't get recognized by this makefile,
# we could make that work, but it would require creating a bunch more rules filled with touch commands, and I don't want to.
# CREATE THE TOOL.
MAIN_CPP_INCLUDES              := src/geometry/mesh_t.h src/texturing/texture_t.h src/entities/entity_t.h src/shaders/entity_shader_t.h src/texturing/skybox_texture_t.h src/skybox/skybox_t.h src/shaders/skybox_shader_t.h src/scene/scene_t.h src/rendering/camera_t.h src/math/vector3f_t.h src/math/vector4f_t.h src/math/matrix4f_t.h src/rendering/entity_renderer_t.h src/rendering/skybox_renderer_t.h src/debug/logger.h src/exit_program.h
DEBUG_LOGGER_CPP_INCLUDES      := src/debug/logger.h
EXIT_PROGRAM_CPP_INCLUDES      := src/exit_program.h
CAMERA_T_CPP_INCLUDES          := src/rendering/camera_t.h src/math/matrix4f_t.h
ENTITY_RENDERER_T_CPP_INCLUDES := src/rendering/renderer_t.h src/rendering/entity_renderer_t.h src/scene/scene_t.h
RENDERER_T_CPP_INCLUDES        := src/rendering/renderer_t.h src/scene/scene_t.h
ENTITY_SHADER_T_CPP_INCLUDES   := src/shaders/entity_shader_t.h src/math/matrix4f_t.h
SHADER_T_CPP_INCLUDES          := src/shaders/shader_t.h src/math/matrix4f_t.h src/debug/logger.h src/exit_program.h
IMPLEMENTATION_CPP_INCLUDES    := src/debug/logger.h src/exit_program.h
MESH_T_CPP_INCLUDES            := src/geometry/mesh_t.h src/math/vector3f_t.h src/math/vector3ui_t.h
SKYBOX_TEXTURE_T_CPP_INCLUDES  := src/texturing/skybox_texture_t.h src/stb/stb_image.h src/debug/logger.h src/exit_program.h

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

CLANG_PREAMBLE := $(CLANG_PROGRAM_NAME) -std=$(CPP_STD) -$(OPTIMIZATION_LEVEL) $(POSSIBLE_WALL) -Wno-missing-braces -fno-exceptions -pthread

.PHONY: all unoptimized clean clean_include_swaps

all: bin/$(BINARY_NAME)

unoptimized:
	$(MAKE) OPTIMIZATION_LEVEL:=O0

bin/$(BINARY_NAME): bin/main.o bin/debug/logger.o bin/exit_program.o bin/rendering/camera_t.o bin/rendering/entity_renderer_t.o bin/rendering/renderer_t.o bin/shaders/entity_shader_t.o bin/shaders/shader_t.o bin/opengl/implementation.o bin/texturing/skybox_texture_t.o bin/shaders/skybox_shader_t.o bin/rendering/skybox_renderer_t.o bin/geometry/mesh_t.o bin/stb/implementation.o bin/baked_assets/skybox_face_images.o
	$(CLANG_PREAMBLE) -o bin/$(BINARY_NAME) bin/*.o bin/debug/*.o bin/rendering/*.o bin/shaders/*.o bin/opengl/*.o bin/geometry/*.o bin/stb/*.o bin/baked_assets/*.o bin/texturing/*.o -lGL -lglfw

bin/main.o: src/main.cpp $(MAIN_CPP_INCLUDES) bin/.dirstamp
	$(CLANG_PREAMBLE) -c -Isrc -o bin/main.o src/main.cpp

bin/debug/logger.o: src/debug/logger.cpp $(DEBUG_LOGGER_CPP_INCLUDES) bin/debug/.dirstamp
	$(CLANG_PREAMBLE) -c -Isrc -o bin/debug/logger.o src/debug/logger.cpp

bin/exit_program.o: src/exit_program.cpp $(EXIT_PROGRAM_CPP_INCLUDES) bin/.dirstamp
	$(CLANG_PREAMBLE) -c -Isrc -o bin/exit_program.o src/exit_program.cpp

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

bin/stb/implementation.o: src/stb/implementation.cpp src/stb/stb_image.h bin/stb/.dirstamp
	$(CLANG_PREAMBLE) -c -Isrc -o bin/stb/implementation.o src/stb/implementation.cpp

bin/texturing/skybox_texture_t.o: src/texturing/skybox_texture_t.cpp $(SKYBOX_TEXTURE_T_CPP_INCLUDES) bin/texturing/.dirstamp
	$(CLANG_PREAMBLE) -c -Isrc -o bin/texturing/skybox_texture_t.o src/texturing/skybox_texture_t.cpp

bin/shaders/skybox_shader_t.o: src/shaders/skybox_shader_t.cpp $(SKYBOX_SHADER_T_CPP_INCLUDES) bin/shaders/.dirstamp
	$(CLANG_PREAMBLE) -c -Isrc -o bin/shaders/skybox_shader_t.o src/shaders/skybox_shader_t.cpp

bin/rendering/skybox_renderer_t.o: src/rendering/skybox_renderer_t.cpp $(SKYBOX_RENDERER_T_CPP_INCLUDES) bin/rendering/.dirstamp
	$(CLANG_PREAMBLE) -c -Isrc -o bin/rendering/skybox_renderer_t.o src/rendering/skybox_renderer_t.cpp

src/baked_assets/skybox_face_images.cpp: assets/skybox_face_image_0.png assets/skybox_face_image_1.png assets/skybox_face_image_2.png assets/skybox_face_image_3.png assets/skybox_face_image_4.png assets/skybox_face_image_5.png
	echo -n "#include \"skybox_face_images.h\"\nnamespace baked_assets {\n" > src/baked_assets/skybox_face_images.cpp
	srcembed --varname skybox_face_image_0_data c++ < assets/skybox_face_image_0.png >> src/baked_assets/skybox_face_images.cpp
	echo -n "const unsigned char *skybox_face_image_0 = skybox_face_image_0_data;\n" >> src/baked_assets/skybox_face_images.cpp
	echo -n "const size_t skybox_face_image_size_0 = sizeof(skybox_face_image_0_data);\n" >> src/baked_assets/skybox_face_images.cpp
	srcembed --varname skybox_face_image_1_data c++ < assets/skybox_face_image_1.png >> src/baked_assets/skybox_face_images.cpp
	echo -n "const unsigned char *skybox_face_image_1 = skybox_face_image_1_data;\n" >> src/baked_assets/skybox_face_images.cpp
	echo -n "const size_t skybox_face_image_size_1 = sizeof(skybox_face_image_1_data);\n" >> src/baked_assets/skybox_face_images.cpp
	srcembed --varname skybox_face_image_2_data c++ < assets/skybox_face_image_2.png >> src/baked_assets/skybox_face_images.cpp
	echo -n "const unsigned char *skybox_face_image_2 = skybox_face_image_2_data;\n" >> src/baked_assets/skybox_face_images.cpp
	echo -n "const size_t skybox_face_image_size_2 = sizeof(skybox_face_image_2_data);\n" >> src/baked_assets/skybox_face_images.cpp
	srcembed --varname skybox_face_image_3_data c++ < assets/skybox_face_image_3.png >> src/baked_assets/skybox_face_images.cpp
	echo -n "const unsigned char *skybox_face_image_3 = skybox_face_image_3_data;\n" >> src/baked_assets/skybox_face_images.cpp
	echo -n "const size_t skybox_face_image_size_3 = sizeof(skybox_face_image_3_data);\n" >> src/baked_assets/skybox_face_images.cpp
	srcembed --varname skybox_face_image_4_data c++ < assets/skybox_face_image_4.png >> src/baked_assets/skybox_face_images.cpp
	echo -n "const unsigned char *skybox_face_image_4 = skybox_face_image_4_data;\n" >> src/baked_assets/skybox_face_images.cpp
	echo -n "const size_t skybox_face_image_size_4 = sizeof(skybox_face_image_4_data);\n" >> src/baked_assets/skybox_face_images.cpp
	srcembed --varname skybox_face_image_5_data c++ < assets/skybox_face_image_5.png >> src/baked_assets/skybox_face_images.cpp
	echo -n "const unsigned char *skybox_face_image_5 = skybox_face_image_5_data;\n" >> src/baked_assets/skybox_face_images.cpp
	echo -n "const size_t skybox_face_image_size_5 = sizeof(skybox_face_image_5_data);\n" >> src/baked_assets/skybox_face_images.cpp
	echo -n "}\n" >> src/baked_assets/skybox_face_images.cpp

bin/baked_assets/skybox_face_images.o: src/baked_assets/skybox_face_images.cpp src/baked_assets/skybox_face_images.h bin/baked_assets/.dirstamp
	$(CLANG_PREAMBLE) -c -o bin/baked_assets/skybox_face_images.o src/baked_assets/skybox_face_images.cpp

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

bin/stb/.dirstamp: bin/.dirstamp
	mkdir -p bin/stb
	touch bin/stb/.dirstamp

bin/texturing/.dirstamp: bin/.dirstamp
	mkdir -p bin/texturing
	touch bin/texturing/.dirstamp

bin/baked_assets/.dirstamp: bin/.dirstamp
	mkdir -p bin/baked_assets
	touch bin/baked_assets/.dirstamp

# The normal clean rule ignores swap files in case you have open vim instances. This is respectful to them.
# Use the clean_include_swaps rule to clean every untracked file. You can do that if you don't have any vim instances open.
clean:
	git clean -fdx -e *.swp

clean_include_swaps:
	git clean -fdx
