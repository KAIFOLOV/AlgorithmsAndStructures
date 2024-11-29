#ifndef HASHFUNCTION_H
#define HASHFUNCTION_H

#include <cstddef>

class HashFunction
{
public:
    virtual ~HashFunction() = default;

    virtual size_t operator()(int key, size_t capacity) const = 0;
};

class DefaultHashFunction : public HashFunction
{
public:
    size_t operator()(const int key, const size_t capacity) const override;
};

#endif // HASHFUNCTION_H
