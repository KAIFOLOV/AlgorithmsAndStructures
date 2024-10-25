#ifndef MULTIPHASESORTER_H
#define MULTIPHASESORTER_H

#include <iostream>
#include <vector>

class MultiPhaseSorter
{
public:
    explicit MultiPhaseSorter() = default;
    void sort(const std::string &inputFile, const int numFiles = 4);

private:
    void initialize();
    void createAuxiliaryFiles();
    void recalculateLevels();

    void splitData(const std::string &inputFile);
    void mergeData();

    void cleanupTempFiles(const std::vector<std::string> &tempFiles);

    std::vector<std::string> _tempFiles;

    std::vector<int> _ip {};
    std::vector<int> _ms {};
    int _numFiles = 0;
    int _L = 0;
};

#endif // MULTIPHASESORTER_H
