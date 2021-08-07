# To build the project and NOT ruin the root-directory with build-files, please use the following:
# cd cmake-build && cmake .. && make
# The included makefile in the root-directory can be used instead to make this process easier
# make build -> for creating a build
# make test -> for creating a build and running `make test` in the build directory
# make coverage -> for creating a build and creating a coverage report from the unit_tests
# make testrun -> for creating a build and running the unit_tests executable
# make cmake-clean -> for cleaning the build directory `cmake-build`

cmake_minimum_required(VERSION 3.10.0)
project(hospital_simulator C CXX)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

enable_testing()
find_package(PkgConfig REQUIRED) # To check for the GTK Module
pkg_check_modules(GTK REQUIRED "gtk+-3.0")
find_package(Threads REQUIRED)

include(${CMAKE_CURRENT_SOURCE_DIR}/modules/GTest_ExternalProject.cmake)
include(GoogleTest)
include(${CMAKE_CURRENT_SOURCE_DIR}/modules/CodeCoverage.cmake)
append_coverage_compiler_flags()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc ${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
add_executable(${PROJECT_NAME} ${SRC_FILES})
add_library(${PROJECT_NAME}_library SHARED ${SRC_FILES})

add_dependencies(${PROJECT_NAME}
    ${PROJECT_NAME}
    libgtest
    libgtest_main
)

file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cxx)
add_executable(${PROJECT_NAME}_unit_tests ${TEST_FILES})
add_dependencies(${PROJECT_NAME}_unit_tests
    ${PROJECT_NAME}_library
    libgtest
    libgtest_main
)

set(WARNING_LEVELS_GCC
    -Werror
)
set(WARNING_LEVELS_GCC_DEBUG
	-Wall
	-Wextra
	-Wno-unused-parameter
	# -pedantic
	-Wno-write-strings
	-Wno-missing-field-initializers
	-g
)

target_link_libraries(${PROJECT_NAME} ${GTK_LDFLAGS})
target_compile_options(${PROJECT_NAME} PRIVATE ${GTK_CFLAGS} ${WARNING_LEVELS_GCC_DEBUG})

target_link_libraries(${PROJECT_NAME}_unit_tests libgtest libgtest_main ${GTK_LDFLAGS} ${PROJECT_NAME}_library)
target_compile_options(${PROJECT_NAME}_unit_tests PRIVATE ${GTEST_CFLAGS} ${WARNING_LEVELS_GCC_DEBUG} ${GTK_CFLAGS})

gtest_discover_tests(${PROJECT_NAME}_unit_tests)
setup_target_for_coverage_gcovr_html(
    NAME ${PROJECT_NAME}_coverage
    EXECUTABLE ctest -j ${PROCESSOR_COUNT}
    DEPENDENCIES ${PROJECT_NAME}_unit_tests
    EXCLUDE "${CMAKE_CURRENT_BINARY_DIR}/gtest/*")