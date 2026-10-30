cmake_minimum_required(VERSION 4.3)
project(erretik
        VERSION 0.0.1
        DESCRIPTION "Modern C++ header-only error handling library"
)

option(ERRETIK_EXAMPLES "Enable erretik test executable?" ON)
option(ERRETIK_FULL "Enables all features of erretik: stacktrace and others. If you're want just use this as boilerplate-less std::error_code wrapper, disable this." ON)

add_library(erretik INTERFACE)
add_library(erretik::erretik ALIAS erretik)
target_compile_features(erretik INTERFACE cxx_std_20)
target_sources(erretik
        INTERFACE FILE_SET HEADERS BASE_DIRS "include" FILES
        "include/erretik/erretik.hpp"
        "include/erretik/error.hpp"
        "include/erretik/error_code.hpp"
        "include/erretik/features_support.hpp"
)

if (ERRETIK_FULL)
    include("${CMAKE_CURRENT_LIST_DIR}/cmake/cpm.cmake")
    CPMAddPackage("gh:jeremy-rifkin/cpptrace@1.0.4")
    target_link_libraries(erretik INTERFACE cpptrace::cpptrace)
    target_compile_definitions(erretik INTERFACE ERRETIK_FULL)
endif ()

if (ERRETIK_EXAMPLES)
    add_executable(erretik_example)
    add_executable(erretik::example ALIAS erretik_example)
    target_sources(erretik_example PRIVATE "examples/erretik_example.cpp")
    target_link_libraries(erretik_example PRIVATE erretik::erretik)
endif ()

include(GNUInstallDirs)
install(
        TARGETS erretik
        EXPORT erretik::erretik
        FILE_SET HEADERS
)