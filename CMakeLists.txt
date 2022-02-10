cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

# Package hunter
include("cmake-modules/HunterGate.cmake")
HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.23.297.tar.gz"
    SHA1 "3319fe6a3b08090df7df98dee75134d68e2ef5a3"
)

project(dom)

# Import cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")

option(LIBBSD "Use BSD library for strlxxx calls" OFF)

# Get Git tags and set lib version
include(SetSharedLibVersion)

# Use GNUInstallDirs to install libraries into correct
# locations on multiple platforms
include(GNUInstallDirs)

# Use strict compile settings
include(SetStrictCompile)

# Debug build
if (CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "Debug build")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${CMAKE_C_FLAGS} -g -O0 -DDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -O0 -DDEBUG")
    if (COVERAGE MATCHES ON)
        message(STATUS "Coverage enabled")
        find_package(PythonInterp)
        set(CMAKE_COVERAGE_FLAGS "-fprofile-arcs -ftest-coverage --coverage")
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${CMAKE_COVERAGE_FLAGS}")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_COVERAGE_FLAGS}")
    endif()
else()
    message(STATUS "Release build")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
endif()

# Find required libraries and packages
if(LIBBSD)
    find_library(bsd libbsd.so)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_BSD")
    list(APPEND LIBRARY_LINKS bsd)
endif()

# Define source files for library
set(DOM_SRC src/dom.c)
set(PARSE_SRC src/parse.c)
set(STR_UTILS_SRC src/str_utils.c)


set(LIBRARY_SRC
${DOM_SRC}
${PARSE_SRC}
${STR_UTILS_SRC}
)

add_library(dom SHARED ${LIBRARY_SRC})

# Define header files for library
target_include_directories(dom PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src
)

# Link libraries
target_link_libraries(dom ${LIBRARY_LINKS})
set_target_properties(dom
    PROPERTIES
    VERSION ${GENERIC_LIB_VERSION}
    SOVERSION ${GENERIC_LIB_SOVERSION}
)

# Add examples
set(html2stream_src
html-to-stream/html2stream.c
html-to-stream/readable.c
)

add_executable(html2stream ${html2stream_src})
target_link_libraries(html2stream dom)

# Install library to GNU locations
install(TARGETS dom EXPORT libdom-config
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Unit tests
enable_testing()
include(GenCtestWithGtest)

gen_ctest_with_gtest(
    COMPONENT_NAME str-utils
    TEST_SOURCE tests/test_str_utils.cpp
    TEST_LINKS ${LIBRARY_LINKS} dom
    EXTRA_INCLUDE src
)

gen_ctest_with_gtest(
    COMPONENT_NAME core
    TEST_SOURCE tests/test_dom.cpp html-to-stream/readable.c
    TEST_LINKS ${LIBRARY_LINKS} dom
    EXTRA_INCLUDE src html-to-stream/
    TEST_ARGS "-c ../tests/html/astro-year.html"
)
