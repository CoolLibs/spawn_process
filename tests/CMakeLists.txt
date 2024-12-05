cmake_minimum_required(VERSION 3.11)
project(spawn_process-tests)

# ---Create executable---
add_executable(${PROJECT_NAME} tests.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# ---Create dummy executable that we will try to spawn in a process---
add_executable(dummy_exe dummy_exe.cpp)
target_compile_features(dummy_exe PRIVATE cxx_std_17)
set_target_properties(dummy_exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
get_target_property(EXECUTABLE_PATH dummy_exe RUNTIME_OUTPUT_DIRECTORY)
set(EXECUTABLE_FULL_PATH "${EXECUTABLE_PATH}/dummy_exe${CMAKE_EXECUTABLE_SUFFIX}")
target_compile_definitions(${PROJECT_NAME} PRIVATE EXECUTABLE_PATH="${EXECUTABLE_FULL_PATH}")
add_dependencies(${PROJECT_NAME} dummy_exe)

# ---Set warning level---
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -pedantic-errors -Wconversion -Wsign-conversion -Wimplicit-fallthrough)
endif()

# ---Maybe enable warnings as errors---
if(WARNINGS_AS_ERRORS_FOR_SPAWN_PROCESS)
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /WX)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -Werror)
    endif()
endif()

# ---Include our library---
add_subdirectory(.. ${CMAKE_CURRENT_SOURCE_DIR}/build/spawn_process)
target_link_libraries(${PROJECT_NAME} PRIVATE Cool::spawn_process)

# ---Ignore .vscode/settings.json in Git---
find_package(Git QUIET)

if(GIT_FOUND)
    get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

    if(EXISTS "${PARENT_DIR}/.git")
        execute_process(COMMAND ${GIT_EXECUTABLE} update-index --assume-unchanged .vscode/settings.json
            WORKING_DIRECTORY ${PARENT_DIR}
            RESULT_VARIABLE ERRORS)

        if(NOT ERRORS EQUAL "0")
            message("Git assume-unchanged failed: ${ERRORS}")
        endif()
    else()
        message("No Git repository found.")
    endif()
else()
    message("Git executable not found.")
endif()
