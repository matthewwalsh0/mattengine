cmake_minimum_required(VERSION 3.10.2)
project(mattengine)
set(CMAKE_CXX_STANDARD 20)

# glfw
add_subdirectory(deps/glfw)

# glm
add_subdirectory(deps/glm)

# glad
include_directories(deps/glad/include)
file(GLOB GLAD "deps/glad/src/*.c" "deps/glad/*.h")

# stb_image
include_directories(deps/stb_image)

# freetype
set(CMAKE_DISABLE_FIND_PACKAGE_HarfBuzz ON)
add_subdirectory(deps/freetype)

# entt
include_directories(deps/entt/single_include/entt)

# assimp
add_subdirectory(deps/assimp)

# imgui
include_directories(deps/imgui)
include_directories(deps/imgui/backends)
file(GLOB IMGUI "deps/imgui/*.cpp" "deps/imgui/backends/imgui_impl_glfw.cpp" "deps/imgui/backends/imgui_impl_opengl3.cpp")

# mattengine
file(GLOB_RECURSE SOURCES "src/engine/**.cpp" "src/engine/**.h", "deps/stb_image/*.cpp", "deps/stb_image/*.h")
include_directories(src/engine)
include_directories(src/engine/components)
add_library(mattengine STATIC ${SOURCES} ${GLAD} ${IMGUI})
target_link_libraries(mattengine glfw glm freetype assimp)

# demo
file(GLOB_RECURSE DEMO_SOURCES "src/demo/**.cpp" "src/demo/**.h")
add_executable(demo ${DEMO_SOURCES})
target_link_libraries(demo mattengine)
