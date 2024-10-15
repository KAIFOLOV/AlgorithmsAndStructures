#ifndef MULTIPHASESORTER_H
#define MULTIPHASESORTER_H

#include <iostream>
#include <vector>

class MultiPhaseSorter
{
public:
    static void sort(const std::string &inputFile, const int numFiles = 3);

private:
    static void createAuxiliaryFiles(std::vector<std::string> &tempFiles, int numFiles);
    static void splitData(const std::string &inputFile, std::vector<std::string> &tempFiles);
    static std::vector<int> generateFibonacciDistribution(const int numFiles);
    static void addFakeSegment(const std::string &fileName);
    static void writeToFile(std::vector<int> &buffer, const std::string &fileName);
    static void multiPhaseMerge(const std::vector<std::string> &tempFiles,
                                const std::string &outputFile);
    static void cleanupTempFiles(const std::vector<std::string> &tempFiles);
};

#endif // MULTIPHASESORTER_H
