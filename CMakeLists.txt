cmake_minimum_required(VERSION 4.3)
project(erretik
        VERSION 0.0.1
        DESCRIPTION "Modern C++ header-only error handling library"
)

option(ERRETIK_EXAMPLES "Enable erretik test executable?" ON)
option(ERRETIK_ENSURE_STACKTRACE "If enabled, fails configuration if <stacktrace> not supported" ON)

add_library(erretik INTERFACE)
add_library(erretik::erretik ALIAS erretik)
target_compile_features(erretik INTERFACE cxx_std_23)
target_sources(erretik
        INTERFACE FILE_SET HEADERS BASE_DIRS "include" FILES
        "include/erretik/erretik.hpp"
        "include/erretik/error.hpp"
        "include/erretik/error_code.hpp"
        "include/erretik/stacktrace_stub.hpp"
)


include(CheckCXXSourceCompiles)
check_cxx_source_compiles(
        [[
                #include <stacktrace>
                int main() { const auto _ = std::stacktrace::current(); }
        ]]
        ERRETIK_HAVE_STD_STACKTRACE
)

if (NOT DEFINED ERRETIK_HAVE_STD_STACKTRACE)
    if (ERRETIK_ENSURE_STACKTRACE)
        message(
                FATAL_ERROR
                [[
                        Cannot build erretik: looks like your compiler/std doesn't support C++23 <stacktrace>.
                        Update your compiler or rerun cmake with '-DERRETIK_ENSURE_STACKTRACE=OFF'
                ]]
        )
    else ()
        message(
                WARNING
                [[
                        Beware: stacktrace replaced with empty stub
                        because your compiler doesn't support C++23 <stacktrace>
                ]]
        )
        target_compile_definitions(erretik PRIVATE ERRETIK_STACKTRACE_UNSUPPORTED)
    endif ()
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