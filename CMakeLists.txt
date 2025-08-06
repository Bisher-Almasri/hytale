cmake_minimum_required(VERSION 3.10)

project(hypickel VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include imgui src)

add_subdirectory(include/tinygltf)

set(SOURCES
  src/main.cpp
  src/Game.hpp
  src/Game.cpp

  src/Helpers/Shader.hpp
  src/Helpers/Shader.cpp
  src/Helpers/Camera.hpp
  src/Helpers/Camera.cpp
  src/Helpers/PerlinNoise.hpp

  src/Rendering/Renderer.hpp
  src/Rendering/Renderer.cpp
  src/Rendering/Chunk.hpp
  src/Rendering/Chunk.cpp
  src/Rendering/World.hpp
  src/Rendering/World.cpp

  src/Rendering/Meshes/GLTFLoader.hpp
  src/Rendering/Meshes/GLTFLoader.cpp
  src/Rendering/Meshes/Mesh.hpp
  src/Rendering/Meshes/Mesh.cpp

  include/glad/glad.c
  # include/stb/stb_image.c
)

# ImGui
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

find_package(OpenGL REQUIRED)


if(APPLE)
  message(STATUS "Building on macOS")

  find_library(COCOA_LIBRARY Cocoa REQUIRED)
  find_library(IOKIT_LIBRARY IOKit REQUIRED)
  find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)

  find_package(PkgConfig REQUIRED)

pkg_check_modules(GLFW REQUIRED glfw3)

pkg_check_modules(GLM REQUIRED glm)

target_include_directories(hypickel PRIVATE ${GLFW_INCLUDE_DIRS})

target_link_directories(hypickel PRIVATE ${GLFW_LIBRARY_DIRS})

target_include_directories(hypickel PRIVATE ${GLM_INCLUDE_DIRS})

target_link_directories(hypickel PRIVATE ${GLM_LIBRARY_DIRS})

  target_link_libraries(hypickel
    ${GLFW_LIBRARIES}
    ${GLM_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${COCOA_LIBRARY}
    ${IOKIT_LIBRARY}
    ${COREVIDEO_LIBRARY}
    tinygltf
  )

else()
  message(STATUS "Building on Linux/Windows")
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
link_directories(${GLFW_LIBRARY_DIRS})

pkg_check_modules(GLM REQUIRED glm)
include_directories(${GLM_INCLUDE_DIRS})
link_directories(${GLM_LIBRARY_DIRS})

  target_link_libraries(hypickel
    ${GLFW_LIBRARIES}
    ${GLM_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
    tinygltf
    dl
  )
endif()
