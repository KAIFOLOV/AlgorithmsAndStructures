#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "HuffmanNode.h"

#include <string>
#include <unordered_map>

class HuffmanCoder
{
public:
    HuffmanCoder();
    ~HuffmanCoder();

    void build(const std::string &input);
    int encodeFile(const std::string &inputFile, const std::string &outputFile);
    bool decodeFile(const std::string &inputFile, const std::string &outputFile);

private:
    HuffmanNode *_root;
    std::unordered_map<char, std::vector<bool>> _codes;
    std::unordered_map<std::vector<bool>, char> _reverseCodes;

    void generateCodes(HuffmanNode *node, std::vector<bool> &currentCode);
    void clearTree();

    std::string readFile(const std::string &filePath);
    void writeBinaryFile(const std::string &filePath, const std::vector<bool> &data);
    std::vector<bool> readBinaryFile(const std::string &filePath);
};

#endif // HUFFMAN_H
