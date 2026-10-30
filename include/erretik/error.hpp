// <=========>
// < erretik >
// <=========>
// Modern C++ error handling library
//
// This project is distributed under the MIT license
// For full license text check LICENSE file
//
// Copyright (c) 2026 username000101 <cxx.username000101@gmail.com>

#pragma once

#include <format>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>

#ifdef ERRETIK_FULL
#include <cpptrace/cpptrace.hpp>
#endif

namespace erretik {
    /// @brief Provides wrapper for error_code, message and stacktrace
    ///
    /// @code
    /// error::make_error(
    ///     generic_error_code<your_error_enum>::make_error_code(your_error_enum::file_not_found),
    ///     "File '{}' does not exist", "SomeFile.txt"
    /// );
    /// @endcode
    class error {
        std::string message;

        std::error_code code;

#ifdef ERRETIK_FULL
        cpptrace::stacktrace stacktrace;
#endif

        error(const decltype(code) error_code, std::string msg)
        : message(std::move(msg)), code(error_code)
        #ifdef ERRETIK_FULL
        , stacktrace(cpptrace::generate_trace(2))
        #endif
        {}
    public:
        [[nodiscard]] std::error_code get_code() const { return code; }
        [[nodiscard]] std::string_view get_message() const { return message; }
#ifdef ERRETIK_FULL
        [[nodiscard]] const cpptrace::stacktrace& get_stacktrace() const { return stacktrace; }
#endif

        template <class... Args>
        [[nodiscard]] static error make_error(
            const decltype(code) error_code,
            std::format_string<Args...> fmt,
            Args&&... args
            ) noexcept {
            return { error_code, std::format(fmt, std::forward<Args>(args)...) };
        }

        [[nodiscard]] static error make_error(
            decltype(code) error_code,
            const std::string& msg,
            const std::format_args format_args) {
            return { error_code, std::vformat(std::move(msg), format_args) };
        }
    };
}