#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>
#include <list>
#include <string>
#include <memory>
#include "HashFunction.h"

class HashTable
{
public:
    struct HashNode
    {
        int key;
        std::string value;
        HashNode(const int key, const std::string &value);
    };

    HashTable(const size_t size = 10, HashFunction *hashFunc = new DefaultHashFunction());
    HashTable(const HashTable &other);

    ~HashTable();

    void setHashFunction(HashFunction *newHashFunction);

    void insert(const int key, const std::string &value);
    void remove(const int key);
    bool contains(const int key) const;

    HashTable &operator=(const HashTable &other);
    std::string &operator[](int key);
    const std::string &operator[](int key) const;

    void printTable() const;

private:
    size_t hash(const int key) const;
    void resize();

    std::vector<std::list<HashNode>> _table;
    size_t _currentSize;
    size_t _capacity;

    HashFunction *_hashFunction;
};
#endif // HASHTABLE_H
