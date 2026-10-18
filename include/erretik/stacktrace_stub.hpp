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

#ifdef ERRETIK_STACKTRACE_UNSUPPORTED

constexpr bool ERRETIK_STACKTRACE_WORKS = false;

namespace erretik {
    struct stacktrace_stub {
        [[nodiscard]] static stacktrace_stub current(int skip_frames) { return {}; }
    };

    using stacktrace = stacktrace_stub;
}

#else

#include <stacktrace>

/// Use this constexpr variable for constexpr conditions with stacktrace
///
/// @code
/// if constexpr (ERRETIK_STACKTRACE_WORKS) { /* stacktrace forks fully */ }
/// else { /* stacktrace works through stub - cannot use this */ }
constexpr bool ERRETIK_STACKTRACE_WORKS = true;

namespace erretik {
    using stacktrace = std::stacktrace;
}

#endif // ERRETIK_STACKTRACE_UNSUPPORTED