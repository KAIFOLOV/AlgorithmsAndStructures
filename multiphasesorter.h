#ifndef MULTIPHASESORTER_H
#define MULTIPHASESORTER_H

#include <iostream>
#include <vector>

class MultiPhaseSorter
{
public:
    static void sort(const std::string &inputFile, const int numFiles = 5);

private:
    static void createAuxiliaryFiles(std::vector<std::string> &tempFiles, int numFiles);
    static void splitData(const std::string &inputFile, std::vector<std::string> &tempFiles);
    static void sortAndWriteToFile(std::vector<int> &buffer, const std::string &fileName);
    static void mergeSortedFiles(const std::vector<std::string> &tempFiles,
                                 const std::string &inputFile);
    static void cleanupTempFiles(const std::vector<std::string> &tempFiles);
};

#endif // MULTIPHASESORTER_H
