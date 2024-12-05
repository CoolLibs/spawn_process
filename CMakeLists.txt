cmake_minimum_required(VERSION 3.8)

set(WARNINGS_AS_ERRORS_FOR_SPAWN_PROCESS OFF CACHE BOOL "ON iff you want to treat warnings as errors")

add_library(spawn_process)
add_library(Cool::spawn_process ALIAS spawn_process)
target_compile_features(spawn_process PUBLIC cxx_std_17)

# ---Add source files---
if(WARNINGS_AS_ERRORS_FOR_SPAWN_PROCESS)
    target_include_directories(spawn_process PUBLIC include)
else()
    target_include_directories(spawn_process SYSTEM PUBLIC include)
endif()

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp)
target_sources(spawn_process PRIVATE ${SRC_FILES})

# ---Set warning level---
if(MSVC)
    target_compile_options(spawn_process PRIVATE /W4)
else()
    target_compile_options(spawn_process PRIVATE -Wall -Wextra -Wpedantic -pedantic-errors -Wconversion -Wsign-conversion -Wimplicit-fallthrough)
endif()

# ---Maybe enable warnings as errors---
if(WARNINGS_AS_ERRORS_FOR_SPAWN_PROCESS)
    if(MSVC)
        target_compile_options(spawn_process PRIVATE /WX)
    else()
        target_compile_options(spawn_process PRIVATE -Werror)
    endif()
endif()
