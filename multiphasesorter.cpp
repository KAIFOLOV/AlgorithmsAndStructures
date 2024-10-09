#include "multiphasesorter.h"
#include <algorithm>
#include <fstream>
#include <limits>
#include <iostream>

void MultiPhaseSorter::sort(const std::string &inputFile, const int numFiles)
{
    std::vector<std::string> tempFiles;
    createAuxiliaryFiles(tempFiles, numFiles);
    splitData(inputFile, tempFiles);

    // Отладочный вывод: содержимое временных файлов после разбиения
    // std::cout << "Temporary files after splitting:" << std::endl;
    // for (const auto &tempFile : tempFiles) {
    //     std::ifstream checkFile(tempFile);
    //     int val;
    //     std::cout << "Contents of " << tempFile << ": ";
    //     while (checkFile >> val) {
    //         std::cout << val << " ";
    //     }
    //     std::cout << std::endl;
    // }

    mergeSortedFiles(tempFiles, inputFile);
    cleanupTempFiles(tempFiles);
}

void MultiPhaseSorter::createAuxiliaryFiles(std::vector<std::string> &tempFiles, int numFiles)
{
    for (int i = 0; i < numFiles; ++i) {
        tempFiles.push_back("temp" + std::to_string(i) + ".txt");
    }
}

void MultiPhaseSorter::splitData(const std::string &inputFile, std::vector<std::string> &tempFiles)
{
    std::ifstream inFile(inputFile);
    if (!inFile) {
        std::cerr << "Error: Could not open input file " << inputFile << std::endl;
        return;
    }

    std::vector<int> buffer;
    int number;
    int fileIndex = 0;

    int estimatedSize = 0;
    while (inFile >> number) {
        ++estimatedSize;
    }

    inFile.clear();
    inFile.seekg(0);

    int maxBufferSize = std::max(100, static_cast<int>(estimatedSize / tempFiles.size()));

    // std::cout << "Buffer size set to: " << maxBufferSize << std::endl;

    while (inFile >> number) {
        buffer.push_back(number);

        if (buffer.size() >= maxBufferSize) {
            sortAndWriteToFile(buffer, tempFiles[fileIndex]);
            fileIndex = (fileIndex + 1) % tempFiles.size();
        }
    }

    if (!buffer.empty()) {
        sortAndWriteToFile(buffer, tempFiles[fileIndex]);
    }
}

void MultiPhaseSorter::sortAndWriteToFile(std::vector<int> &buffer, const std::string &fileName)
{
    std::sort(buffer.begin(), buffer.end());
    std::ofstream outFile(fileName);
    for (int num : buffer) {
        outFile << num << " ";
    }
    buffer.clear();
}

void MultiPhaseSorter::mergeSortedFiles(const std::vector<std::string> &tempFiles,
                                        const std::string &inputFile)
{
    std::vector<std::ifstream> fileStreams;
    for (const auto &fileName : tempFiles) {
        fileStreams.emplace_back(fileName);
    }

    std::ofstream outFile(inputFile);
    bool finished = false;
    std::vector<int> currentNumbers(tempFiles.size(), std::numeric_limits<int>::max());

    for (int i = 0; i < tempFiles.size(); ++i) {
        if (fileStreams[i] >> currentNumbers[i]) {
            continue;
        }
    }

    while (!finished) {
        int minNumber = std::numeric_limits<int>::max();
        int minIndex = -1;

        for (int i = 0; i < tempFiles.size(); ++i) {
            if (currentNumbers[i] < minNumber) {
                minNumber = currentNumbers[i];
                minIndex = i;
            }
        }

        if (minIndex == -1) {
            finished = true;
        } else {
            outFile << minNumber << " ";
            if (!(fileStreams[minIndex] >> currentNumbers[minIndex])) {
                currentNumbers[minIndex] =
                 std::numeric_limits<int>::max();
            }
        }
    }
}

void MultiPhaseSorter::cleanupTempFiles(const std::vector<std::string> &tempFiles)
{
    for (const auto &fileName : tempFiles) {
        std::remove(fileName.c_str());
    }
}
