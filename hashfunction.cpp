#include "hashfunction.h"

size_t DefaultHashFunction::operator()(const int key, const size_t capacity) const
{
    return ((key % capacity) + (1 + key % (capacity - 2))) % capacity;
}