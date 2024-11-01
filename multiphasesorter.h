#ifndef MULTIPHASESORTER_H
#define MULTIPHASESORTER_H

#include <iostream>
#include <limits>
#include <vector>

constexpr int SEGMENT_DELIMITER = std::numeric_limits<int>::min();

class MultiPhaseSorter
{
public:
    explicit MultiPhaseSorter() = default;
    void sort(const std::string &inputFile, const int numFiles = 4);

    void rotateFiles(std::vector<std::fstream> &files);

    void writeToOutput(const std::string &inputFile);

    int min(std::vector<int> &segment);

private:
    void initialize();
    void createAuxiliaryFiles();
    void recalculateLevels();

    bool splitData(const std::string &inputFile);
    void merge();
    void mergeFile(std::vector<std::fstream *> &files);

    void cleanupTempFiles();

    std::vector<std::string> _tempFiles;

    std::vector<int> _ip {};
    std::vector<int> _ms {};
    int _numFiles = 0;
    int _L = 0;
};

#endif // MULTIPHASESORTER_H
