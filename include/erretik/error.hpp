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

namespace erretik {
    /// @brief Provides wrapper for error_code, message and stacktrace
    ///
    /// @code
    /// error::make_error(
    ///     generic_error_code<your_error_enum>::make_error_code(your_error_enum::file_not_found),
    ///     "File '{}' does not exist", "SomeFile.txt"
    /// );
    class error {
        stacktrace stacktrace;

        std::string message;

        std::error_code code;

        error(const decltype(code) error_code, std::string msg)
        : stacktrace(stacktrace::current(1)), message(std::move(msg)), code(error_code) {}
    public:
        [[nodiscard]] std::error_code get_code() const { return code; }
        [[nodiscard]] std::string_view get_message() const { return message; }
        [[nodiscard]] const erretik::stacktrace& get_stacktrace() const { return stacktrace; }

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