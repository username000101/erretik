#pragma once

namespace erretik {
    constexpr bool STACKTRACE_SUPPORTED =
#ifdef ERRETIK_FULL
        true
#else
        false
#endif
    ;
}