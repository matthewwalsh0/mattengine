cmake_minimum_required(VERSION 3.10.2)
project(mattengine)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-faligned-allocation)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/static/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/dynamic/${CMAKE_BUILD_TYPE})

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

# physx
set(NV_USE_DEBUG_WINCRT OFF CACHE BOOL "Use the debug version of the CRT")
set(PHYSX_ROOT_DIR ${CMAKE_SOURCE_DIR}/deps/physx/physx)
set(PXSHARED_PATH ${PHYSX_ROOT_DIR}/../pxshared)
set(PXSHARED_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(CMAKEMODULES_VERSION "1.27")
set(CMAKEMODULES_PATH ${PHYSX_ROOT_DIR}/../externals/cmakemodules)
set(TARGET_BUILD_PLATFORM "mac")
set(PX_BUILDSNIPPETS OFF CACHE BOOL "Generate the snippets")
set(PX_BUILDPUBLICSAMPLES OFF CACHE BOOL "Generate the samples projects")
set(PX_OUTPUT_LIB_DIR ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/physx)
set(PX_OUTPUT_BIN_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/physx)
set(PX_GENERATE_STATIC_LIBRARIES ON CACHE BOOL "Generate static libraries")
set(NV_FORCE_64BIT_SUFFIX ON CACHE BOOL "64 bit")
add_subdirectory(deps/physx/physx/compiler/public)
include_directories(deps/physx/physx/include)
include_directories(deps/physx/pxshared/include)
include_directories(deps/physx/physx/snippets)

# mattengine
file(GLOB_RECURSE SOURCES "src/engine/**.cpp" "src/engine/**.h", "deps/stb_image/*.cpp", "deps/stb_image/*.h")
include_directories(src/engine)
include_directories(src/engine/components)
add_library(mattengine STATIC ${SOURCES} ${GLAD} ${IMGUI})
target_link_libraries(mattengine glfw glm freetype assimp PhysXCharacterKinematic PhysXCommon PhysXCooking PhysXExtensions PhysXFoundation PhysXPvdSDK PhysXVehicle PhysX)

# demo
file(GLOB_RECURSE DEMO_SOURCES "src/demo/**.cpp" "src/demo/**.h")
add_executable(demo ${DEMO_SOURCES})
target_link_libraries(demo mattengine)

# physics demo
file(GLOB_RECURSE PHYSICS_DEMO_SOURCES "src/demo_physics/**.cpp" "src/demo_physics/**.h")
add_executable(demo_physics ${PHYSICS_DEMO_SOURCES})
target_link_libraries(demo_physics mattengine)
