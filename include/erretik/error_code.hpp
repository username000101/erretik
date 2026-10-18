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

#include <system_error>

namespace erretik {
    /// @brief
    /// meta-type for users <br>
    /// should implement following interface: <br>
    /// => const char* name - error category name <br>
    /// => ... descriptions - map { your_error_code, description } <br>
    /// => => you should implement your-own helper type like `description_entry` in example <br>
    /// => => this type should provide two fields: <br>
    /// => => .first - `your_error_enum` <br>
    /// => => .second - `const char*` <br>
    /// => ... conversions - map { your_error_code, std::errc } <br>
    /// => => your should implement your-own helper type like `conversion_entry` in example <br>
    /// => => this type should provide two fields: <br>
    /// => => .first - `your_error_enum` <br>
    /// => => .second - `std::errc` <br>
    ///
    /// @code
    /// template <>
    /// struct erretik::error_t<your_error_enum> {
    ///     static constexpr const char* name = "my_awesome_error";
    ///
    ///     struct description_entry {
    ///         your_error_enum error_code;
    ///         const char* description;
    ///     };
    ///
    ///     struct conversion_entry {
    ///         your_error_enum error_code;
    ///         std::errc errc;
    ///     };
    ///
    ///     static constexpr description_entry descriptions[] = {
    ///         {
    ///             your_error_enum::error1,
    ///             "There's error1",
    ///         },
    ///         {
    ///             your_error_enum::error2,
    ///             "And there's error2",
    ///         },
    ///     };
    ///
    ///     static constexpr conversion_entry conversions[] = {
    ///         {
    ///             your_error_enum::file_not_found,
    ///             std::errc::no_such_file_or_directory,
    ///         },
    ///         {
    ///             your_error_enum::file_too_large,
    ///             std::errc::file_too_large,
    ///         },
    ///     };
    /// };
    template <typename ErrorEnum>
    struct error_t;

    /// @brief Generic error code class, also provides error_category functionality
    ///
    /// @code
    /// generic_error_code<your_error_enum>::make_error_code(your_error_enum::error1)
    template <typename ErrorEnum>
    struct generic_error_code : std::error_category {
        [[nodiscard]] const char* name() const noexcept override {
            return error_t<ErrorEnum>::name;
        }

        [[nodiscard]] static const std::error_category& error_category() {
            static const std::error_category& category = generic_error_code();
            return category;
        }

        [[nodiscard]] std::string message(int error_code) const override {
            for (const auto& [code, description] : error_t<ErrorEnum>::descriptions)
                if (static_cast<int>(code) == error_code) return std::string(description);
            return "unknown error";
        }

        [[nodiscard]] std::error_condition default_error_condition(int error_code) const noexcept override {
            for (const auto& [code, conversion] : error_t<ErrorEnum>::conversions)
                if (static_cast<int>(code) == error_code) return std::make_error_condition(conversion);
            return { error_code, error_category() };
        }

        static std::error_code make_error_code(ErrorEnum error_code) {
            return { static_cast<int>(error_code), error_category() };
        }
    };
}