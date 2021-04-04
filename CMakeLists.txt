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

# mattengine
file(GLOB_RECURSE SOURCES "src/engine/**.cpp" "src/engine/**.h", "deps/stb_image/*.cpp", "deps/stb_image/*.h")
include_directories(src/engine)
include_directories(src/engine/components)
add_library(mattengine STATIC ${SOURCES} ${GLAD})
target_link_libraries(mattengine glfw glm freetype)

# demo
file(GLOB_RECURSE DEMO_SOURCES "src/demo/**.cpp" "src/demo/**.h")
add_executable(demo ${DEMO_SOURCES})
target_link_libraries(demo mattengine)
