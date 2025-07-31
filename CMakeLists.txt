cmake_minimum_required(VERSION 3.10)

project(hypickel VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include imgui src)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

add_subdirectory(include/tinygltf)

set(SOURCES
  src/main.cpp
  src/Game.hpp
  src/Game.cpp

  src/Helpers/Shader.hpp
  src/Helpers/Shader.cpp
  src/Helpers/Camera.hpp
  src/Helpers/Camera.cpp

  src/Rendering/Renderer.hpp
  src/Rendering/Renderer.cpp
  src/Rendering/Chunk.hpp
  src/Rendering/Chunk.cpp
  src/Rendering/ChunkManager.hpp
  src/Rendering/ChunkManager.cpp


  src/Rendering/Meshes/GLTFLoader.hpp
  src/Rendering/Meshes/GLTFLoader.cpp
  src/Rendering/Meshes/Mesh.hpp
  src/Rendering/Meshes/Mesh.cpp

  include/glad/glad.c
  #  include/stb/stb_image.c
)

set(IMGUI_SOURCES
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_demo.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

add_executable(hypickel ${SOURCES} ${IMGUI_SOURCES})

include_directories(${GLFW_INCLUDE_DIRS})

target_link_libraries(hypickel ${GLFW_LIBRARIES})
if (POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif ()

find_package(OpenGL REQUIRED)
target_link_libraries(hypickel tinygltf ${OPENGL_gl_LIBRARY} dl)
