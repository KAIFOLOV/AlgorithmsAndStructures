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
        HashNode(int k, const std::string &v);
    };

    HashTable(size_t size = 10, HashFunction *hashFunc = new DefaultHashFunction());
    HashTable(const HashTable &other);

    ~HashTable();

    HashTable &operator=(const HashTable &other);

    void insert(int key, const std::string &value);
    void remove(int key);
    bool contains(int key) const;
    std::string &operator[](int key);
    void printTable() const;

    void setHashFunction(HashFunction *newHashFunction);

private:
    size_t hash(const int key) const;
    void resize();

    std::vector<std::list<HashNode>> _table;
    size_t _currentSize;
    size_t _capacity;

    HashFunction *_hashFunction;
};
#endif // HASHTABLE_H
