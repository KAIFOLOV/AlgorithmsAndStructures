#include "hashtable.h"

#include <iostream>
#include <stdexcept>

HashTable::HashNode::HashNode(const int key, const std::string &value) : key(key), value(value)
{}

HashTable::HashTable(const size_t size, HashFunction *hashFunc) :
    _currentSize(0),
    _capacity(size),
    _hashFunction(hashFunc)
{
    _table.resize(_capacity);
}

HashTable::HashTable(const HashTable &other) :
    _currentSize(other._currentSize),
    _capacity(other._capacity),
    _hashFunction(other._hashFunction),
    _table(other._table)
{}

HashTable::~HashTable()
{}

HashTable &HashTable::operator=(const HashTable &other)
{
    if (this != &other) {
        _capacity = other._capacity;
        _currentSize = other._currentSize;
        _hashFunction = other._hashFunction;
        _table = other._table;
    }
    return *this;
}

size_t HashTable::hash(const int key) const
{
    return (*_hashFunction)(key, _capacity);
}

void HashTable::insert(const int key, const std::string &value)
{
    if (_currentSize >= _capacity) {
        resize();
    }

    size_t index = hash(key);
    for (auto &node : _table[index]) {
        if (node.key == key) {
            node.value = value;
            return;
        }
    }

    _table[index].push_back(HashNode(key, value));
    ++_currentSize;
}

void HashTable::remove(const int key)
{
    size_t index = hash(key);
    auto &chain = _table[index];
    for (auto it = chain.begin(); it != chain.end(); ++it) {
        if (it->key == key) {
            chain.erase(it);
            --_currentSize;
            return;
        }
    }
    throw std::out_of_range("Key not found");
}

bool HashTable::contains(const int key) const
{
    size_t index = hash(key);
    for (const auto &node : _table[index]) {
        if (node.key == key) {
            return true;
        }
    }
    return false;
}

std::string &HashTable::operator[](int key)
{
    size_t index = hash(key);
    for (auto &node : _table[index]) {
        if (node.key == key) {
            return node.value;
        }
    }

    insert(key, "");
    return (*this)[key];
}

const std::string &HashTable::operator[](int key) const
{
    size_t index = hash(key);
    for (auto &node : _table[index]) {
        if (node.key == key) {
            return node.value;
        }
    }

    throw std::runtime_error("Key not exist");
}

void HashTable::resize()
{
    _capacity *= 2;
    std::vector<std::list<HashNode>> oldTable(_capacity);
    _table.swap(oldTable);
    _currentSize = 0;

    for (const auto &chain : oldTable) {
        for (const auto &node : chain) {
            insert(node.key, node.value);
        }
    }
}

void HashTable::printTable() const
{
    for (size_t i = 0; i < _capacity; ++i) {
        if (!_table[i].empty()) {
            std::cout << "Index " << i << ": ";
            for (const auto &node : _table[i]) {
                std::cout << "(" << node.key << ", " << node.value << ") ";
            }
            std::cout << std::endl;
        }
    }
}

void HashTable::setHashFunction(HashFunction *newHashFunction)
{
    _hashFunction = newHashFunction;
    resize();
}
