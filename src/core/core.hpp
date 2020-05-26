#pragma once
#include <fmt/core.h>

namespace cx {

    using uint = unsigned int;
    using usize = unsigned long;
    using byte = unsigned char;

    template <typename ...Args>
    void println(const char* message, Args... args) {
        fmt::print(message, args...);
        fmt::print("\n");
    }

    /// panics with an output to stdout, and calls std::abort() to terminate
    template <typename ...Args>
    void panic(const char* message, Args... args) {
        fmt::print("Panicked, aborting! Panic message: ");
        fmt::print(message, args...);
        fmt::print("\nExiting.");
        std::abort();
    }
    /// Helper/utility debug function. placing this inside a function body, will call panic, and abort program.
    /// Similar to the rust macro unimplemented!("message");
    void unimplemented(const char* signature);
    #define STDOUTLOG(message) fmt::print("Console logging: {}\n", message)
}