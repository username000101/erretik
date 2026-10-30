#include <erretik/erretik.hpp>
#include <erretik/features_support.hpp>

enum class erretik_err { file_not_found, magic_mismatch };

template <>
struct std::is_error_code_enum<erretik_err> : std::true_type {};

std::error_code make_error_code(erretik_err err) {
    return erretik::generic_error_code<erretik_err>::make_error_code(err);
}


template <>
struct erretik::error_t<erretik_err> {
    static constexpr auto name = "erretik";

    struct description_entry { erretik_err err; const char* description; };
    static constexpr description_entry descriptions[] = {
        {
            erretik_err::file_not_found,
            "File not found",
        },
        {
            erretik_err::magic_mismatch,
            "Magic mismatch",
            }
    };

    struct conversion_entry { erretik_err err; std::errc errc_err; };
    static constexpr conversion_entry conversions[] = {
        {
            erretik_err::file_not_found,
            std::errc::no_such_file_or_directory,
        },
    };
};

int main() {
    // Usage variant 1: just as a std::error_code wrapper:
    const auto errc = make_error_code(erretik_err::file_not_found);
    static_cast<void>(errc); // just for suppressing "unused variable" warning
    // and just.. working with it


    // Usage variant 2: as a error with context:
    const auto error = erretik::error::make_error(
        make_error_code(erretik_err::file_not_found),
        "File '{}' does not exist",
        "FILE.txt"
    );
    // use error.get_stacktrace() to get.. stacktrace!
    // use error.get_message() to get message
    // etc.

    // some features may be unavailable - check this with erretik/features_support.hpp:
    if constexpr(erretik::STACKTRACE_SUPPORTED) {
        // stacktrace supported here
        error.get_stacktrace().print_with_snippets();
    }

    // or through macros:
#ifdef ERRETIK_FULL
    // stacktrace supported here
#endif

}