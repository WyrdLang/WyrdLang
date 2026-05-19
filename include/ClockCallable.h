#ifndef CLOCKCALLABLE_H
#define CLOCKCALLABLE_H

#include <any>
#include <vector>
#include <string>
#include <chrono>
#include "WyrdCallable.h"
#include "Interpreter.h"

class ClockCallable : public WyrdCallable {
public:
    // Returns number of expected arguments (same as Java's arity())
    int arity() const override {
        return 0;
    }

    // Called when executing the function in Lox
    // NOTE: Removed 'const' from vector parameter to match base class
    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override {
        using namespace std::chrono;
        auto now = system_clock::now();
        auto seconds = duration_cast<milliseconds>(now.time_since_epoch()).count() / 1000.0;
        return seconds; // returns double
    }

    // String representation (for printing)
    std::string toString() const override {
        return "<native fn>";
    }
};

#endif // CLOCKCALLABLE_H