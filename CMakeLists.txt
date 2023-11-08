cmake_minimum_required(VERSION 3.12)

project(ganim VERSION 0.1)

set(default_build_type "Debug")
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
        STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

add_compile_options(-Wall -Wextra -pedantic -fmax-errors=1)
add_link_options(-Wall -Wextra -pedantic -fmax-errors=1)
add_link_options(-fuse-ld=mold)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-ggdb3 -fsanitize=address -fno-omit-frame-pointer
        -fsanitize=leak -fsanitize=undefined)
    add_link_options(-ggdb3 -fsanitize=address -fno-omit-frame-pointer
        -fsanitize=leak -fsanitize=undefined)
    add_link_options(-Wl,--gdb-index)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "RelWithDebugInfo")
    add_compile_options(-ggdb3 -O2 -flto)
    add_link_options(-ggdb3 -O2 -flto)
    add_link_options(-Wl,--gdb-index)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O2 -flto)
    add_link_options(-O2 -flto)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

find_package(Catch2 3 REQUIRED)

file(GLOB_RECURSE GANIM_FILES CONFIGURE_DEPENDS src/ganim/*.cpp)
file(GLOB_RECURSE GANIM_EXE_FILES CONFIGURE_DEPENDS src/ganim_exe/*.cpp)
file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS src/test/*.cpp)

add_library(ganim SHARED ${GANIM_FILES})
add_library(ganim_exe_common ${GANIM_EXE_FILES})
add_executable(ganim_exe src/ganim_main.cpp)
add_executable(test ${TEST_FILES})

target_include_directories(ganim PUBLIC src)
target_include_directories(ganim_exe PUBLIC src)
target_include_directories(test PUBLIC src)

target_link_libraries(ganim_exe PUBLIC ganim)
target_link_libraries(ganim_exe PUBLIC ganim_exe_common)
target_link_libraries(test PUBLIC ganim)
target_link_libraries(test PUBLIC ganim_exe_common)
target_link_libraries(test PRIVATE Catch2::Catch2WithMain)
