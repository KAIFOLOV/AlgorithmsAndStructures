#include "Huffman.h"

#include <cstdint>
#include <fstream>
#include <queue>

HuffmanCoder::HuffmanCoder() : _root(nullptr)
{}

HuffmanCoder::~HuffmanCoder()
{
    clearTree();
}

void HuffmanCoder::clearTree()
{
    delete _root;
    codes.clear();
    reverseCodes.clear();
}

void HuffmanCoder::build(const std::string &input)
{
    clearTree();

    std::unordered_map<char, int> frequency;
    for (char c : input) {
        frequency[c]++;
    }

    auto compare = [](const HuffmanNode *a, const HuffmanNode *b) {
        return a->frequency() > b->frequency();
    };
    std::priority_queue<HuffmanNode *, std::vector<HuffmanNode *>, decltype(compare)> pq(compare);

    for (const auto &pair : frequency) {
        auto node = new HuffmanNode;
        node->setSymbol(pair.first);
        node->setFrequency(pair.second);
        pq.push(node);
    }

    while (pq.size() > 1) {
        auto left = pq.top();
        pq.pop();
        auto right = pq.top();
        pq.pop();
        auto parent = new HuffmanNode();
        parent->setSymbol('\0');
        parent->setFrequency(left->frequency() + right->frequency());

        parent->setLeftChild(left);
        parent->setRightChild(right);
        pq.push(parent);
    }

    _root = pq.top();

    std::vector<bool> currentCode;
    generateCodes(_root, currentCode);
}

void HuffmanCoder::generateCodes(HuffmanNode *node, std::vector<bool> &currentCode)
{
    if (!node)
        return;

    if (!node->getLeftChild() && !node->getRightChild()) {
        codes[node->symbol()] = currentCode;
        reverseCodes[currentCode] = node->symbol();
    } else {
        currentCode.push_back(false);
        generateCodes(static_cast<HuffmanNode *>(node->getLeftChild()), currentCode);
        currentCode.pop_back();

        currentCode.push_back(true);
        generateCodes(static_cast<HuffmanNode *>(node->getRightChild()), currentCode);
        currentCode.pop_back();
    }
}

int HuffmanCoder::encodeFile(const std::string &inputFile, const std::string &outputFile)
{
    std::string input = readFile(inputFile);
    if (input.empty())
        return -1;

    if (codes.empty())
        build(input);

    std::vector<bool> encodedData;
    for (char c : input) {
        const auto &code = codes[c];
        encodedData.insert(encodedData.end(), code.begin(), code.end());
    }

    writeBinaryFile(outputFile, encodedData);
    return static_cast<int>((1.0 - static_cast<double>(encodedData.size()) / (input.size() * 8))
                            * 100);
}

bool HuffmanCoder::decodeFile(const std::string &inputFile, const std::string &outputFile)
{
    auto encodedData = readBinaryFile(inputFile);
    if (encodedData.empty() || reverseCodes.empty())
        return false;

    std::string decodedData;
    std::vector<bool> currentCode;
    for (bool bit : encodedData) {
        currentCode.push_back(bit);
        if (reverseCodes.count(currentCode)) {
            decodedData += reverseCodes[currentCode];
            currentCode.clear();
        }
    }

    std::ofstream outFile(outputFile);
    if (!outFile)
        return false;

    outFile << decodedData;
    outFile.close();
    return true;
}

std::string HuffmanCoder::readFile(const std::string &filePath)
{
    std::ifstream inFile(filePath, std::ios::in | std::ios::binary);
    if (!inFile)
        return "";

    return std::string(std::istreambuf_iterator<char>(inFile), std::istreambuf_iterator<char>());
}

void HuffmanCoder::writeBinaryFile(const std::string &filePath, const std::vector<bool> &data)
{
    std::ofstream outFile(filePath, std::ios::out | std::ios::binary);
    if (!outFile)
        return;

    for (size_t i = 0; i < data.size(); i += 8) {
        uint8_t byte = 0;
        for (size_t j = 0; j < 8 && i + j < data.size(); ++j) {
            byte |= (data[i + j] << (7 - j));
        }
        outFile.put(byte);
    }
}

std::vector<bool> HuffmanCoder::readBinaryFile(const std::string &filePath)
{
    std::ifstream inFile(filePath, std::ios::in | std::ios::binary);
    if (!inFile)
        return {};

    std::vector<bool> data;
    char byte;
    while (inFile.get(byte)) {
        for (int i = 7; i >= 0; --i) {
            data.push_back((byte >> i) & 1);
        }
    }
    return data;
}
