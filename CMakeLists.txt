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

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED 23)

add_executable(sample_parse src/parse/parse.cpp)
add_custom_command(
    OUTPUT ${PROJECT_BINARY_DIR}/sample_parser.cpp
           ${PROJECT_BINARY_DIR}/sample_parser.hpp
    DEPENDS sample_parse src/parse/sample_grammar.grammar
    COMMAND sample_parse ${PROJECT_SOURCE_DIR}/src/parse/sample_grammar.grammar
                         ${PROJECT_SOURCE_DIR}/src/parse/skeleton
                         ${PROJECT_BINARY_DIR}/sample_parser
)

add_custom_command(
    OUTPUT ${PROJECT_BINARY_DIR}/gex_dimension_parser.cpp
           ${PROJECT_BINARY_DIR}/gex_dimension_parser.hpp
    DEPENDS sample_parse src/parse/gex_dimension.grammar
    COMMAND sample_parse ${PROJECT_SOURCE_DIR}/src/parse/gex_dimension.grammar
                         ${PROJECT_SOURCE_DIR}/src/parse/skeleton
                         ${PROJECT_BINARY_DIR}/gex_dimension_parser
)

find_package(Catch2 3 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(FT2 freetype2)

find_library(AVCODEC avcodec)
find_library(AVUTIL avutil)
find_library(AVFORMAT avformat)
find_library(SWSCALE swscale)
find_library(SFML SFML::Window)
find_library(OPENGL GL)
find_library(HARFBUZZ harfbuzz)

file(GLOB_RECURSE GANIM_FILES CONFIGURE_DEPENDS src/ganim/*.cpp)
file(GLOB_RECURSE GANIM_SCRIPT_FILES CONFIGURE_DEPENDS src/script/*.cpp)
file(GLOB_RECURSE GANIM_EDITOR_FILES CONFIGURE_DEPENDS src/editor/*.cpp)
file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS src/test/*.cpp)
file(GLOB_RECURSE EXAMPLE_FILES CONFIGURE_DEPENDS src/examples/*.cpp)

add_library(ganim STATIC ${GANIM_FILES}
                         ${PROJECT_BINARY_DIR}/gex_dimension_parser.cpp)
add_library(ganimscript OBJECT ${GANIM_SCRIPT_FILES})
add_library(ganimeditor OBJECT ${GANIM_EDITOR_FILES})
add_executable(ganimscript_exe src/script_main.cpp)
add_executable(ganim_exe src/editor_main.cpp)
add_executable(test ${TEST_FILES} ${PROJECT_BINARY_DIR}/sample_parser.cpp)
add_executable(examples ${EXAMPLE_FILES})

target_include_directories(ganim PUBLIC src ${PROJECT_BINARY_DIR})
target_include_directories(ganim PUBLIC ${FT2_INCLUDE_DIRS})
target_compile_options(ganim PUBLIC ${FT2_CFLAGS_OTHER})
target_include_directories(ganimscript PUBLIC src)
target_include_directories(ganimeditor PUBLIC src)
target_include_directories(ganimscript_exe PUBLIC src)
target_include_directories(ganim_exe PUBLIC src)
target_include_directories(test PUBLIC src)
target_include_directories(test PUBLIC src ${PROJECT_BINARY_DIR})
target_include_directories(examples PUBLIC src)

target_link_libraries(ganim PUBLIC "${AVCODEC}")
target_link_libraries(ganim PUBLIC "${AVUTIL}")
target_link_libraries(ganim PUBLIC "${AVFORMAT}")
target_link_libraries(ganim PUBLIC "${SWSCALE}")
target_link_libraries(ganim PUBLIC "${SFML}")
target_link_libraries(ganim PUBLIC "${OPENGL}")
target_link_libraries(ganim PUBLIC "${FT2_LIBRARIES}")
target_link_libraries(ganim PUBLIC "${HARFBUZZ}")
target_link_libraries(ganimscript PRIVATE ganim)
target_link_libraries(ganimeditor PRIVATE ganim)
target_link_libraries(ganimeditor PRIVATE ganimscript)
target_link_libraries(ganimscript_exe PRIVATE ganim)
target_link_libraries(ganimscript_exe PRIVATE ganimscript)
target_link_libraries(ganim_exe PRIVATE ganim)
target_link_libraries(ganim_exe PRIVATE ganimscript)
target_link_libraries(ganim_exe PRIVATE ganimeditor)
target_link_libraries(test PRIVATE ganim)
target_link_libraries(test PRIVATE ganimscript)
target_link_libraries(test PRIVATE ganimeditor)
target_link_libraries(test PRIVATE Catch2::Catch2WithMain)
target_link_libraries(examples PRIVATE ganim)

set_target_properties(ganimscript_exe PROPERTIES OUTPUT_NAME ganimscript)
set_target_properties(ganim_exe PROPERTIES OUTPUT_NAME ganim)
