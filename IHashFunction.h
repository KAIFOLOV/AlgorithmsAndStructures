#ifndef IHASHFUNCTION_H
#define IHASHFUNCTION_H

#include <iostream>

class IHashFunction {
public:
    virtual size_t operator()(const int key, const int attempt, const int n) const = 0;
    virtual ~IHashFunction() = default;
};

class CustomHashFunction : public IHashFunction {
public:
    size_t operator()(const int key, const int attempt, const int n) const override {
        if (n < 2) {
            throw std::invalid_argument("n must be at least 2");
        }

        size_t hash = (key % n + attempt * (1 + key % (n - 2))) % n;
        return hash;
    }
};

#endif // IHASHFUNCTION_H
