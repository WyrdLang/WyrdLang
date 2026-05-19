
#ifndef RETURN_EXCEPTION_H
#define RETURN_EXCEPTION_H

#include <any>
#include <stdexcept>

// A custom exception used to unwind the call stack when returning from a function.
// Note: This is different from the Return statement class in Stmt.h
class ReturnException : public std::runtime_error {
public:
    std::any value;

    // Constructor takes the return value
    explicit ReturnException(std::any value)
        : std::runtime_error(""), value(std::move(value)) {}
};

#endif // RETURN_EXCEPTION_H