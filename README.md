## erretik - an attempt to make the creation of error types more declarative.

Are you also tired of writing a bunch of boilerplates for every type of error in your project?

Don’t you also think that std::error_code is not enough to represent the fact of an error and the context?

erretik is trying to solve both problems:
- declarative structures for defining their own std::error_code descendants
- a general error type that stores the code, message, error location, and stack trace

[Look](examples/erretik_example.cpp) how simple it is

> [!IMPORTANT]
erretik uses std::stacktrace for the stack trace; if your compiler/std does not support \<stacktrace>, erretik may provide a stub; to do this, restart cmake with ```-DERRETIK_ENSURE_STACKTRACE=NO```