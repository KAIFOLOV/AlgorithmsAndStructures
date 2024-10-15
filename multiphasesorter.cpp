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

#ifdef DEBUG
    std::cout << "Temporary files after splitting:" << std::endl;
    for (const auto &tempFile : tempFiles) {
        std::ifstream checkFile(tempFile);
        int val;
        std::cout << "Contents of " << tempFile << ": ";
        while (checkFile >> val) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
#endif

    multiPhaseMerge(tempFiles, inputFile);
    cleanupTempFiles(tempFiles);
}

std::vector<int> MultiPhaseSorter::generateFibonacciDistribution(const int numFiles)
{
    std::vector<int> fibSeries = { 1, 1 };
    while (fibSeries.size() < numFiles) {
        fibSeries.push_back(fibSeries[fibSeries.size() - 1] + fibSeries[fibSeries.size() - 2]);
    }
    return fibSeries;
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

    const auto fibSeries = generateFibonacciDistribution(tempFiles.size());

    int number;
    std::vector<int> buffer;

    for (int fileIndex = 0; fileIndex < tempFiles.size(); ++fileIndex) {
        int currentSeriesSize = fibSeries[fileIndex];
        buffer.clear();

        while (buffer.size() < currentSeriesSize && inFile >> number) {
            buffer.push_back(number);
        }

        if (!buffer.empty()) {
            std::sort(buffer.begin(), buffer.end());
            writeToFile(buffer, tempFiles[fileIndex]);
        }

        while (buffer.size() < currentSeriesSize) {
            addFakeSegment(tempFiles[fileIndex]);
            ++currentSeriesSize;
        }
    }

    while (inFile >> number) {
        buffer.push_back(number);
    }

    if (!buffer.empty()) {
        std::sort(buffer.begin(), buffer.end());
        writeToFile(buffer, tempFiles.back());
    }
}

void MultiPhaseSorter::addFakeSegment(const std::string &fileName)
{
    std::ofstream outFile(fileName, std::ios::app);
    if (!outFile) {
        std::cerr << "Error: Could not open file " << fileName << " to add dummy segment."
                  << std::endl;
        return;
    }

    outFile << std::numeric_limits<int>::max() << " ";
    outFile << std::endl;
}

void MultiPhaseSorter::writeToFile(std::vector<int> &buffer, const std::string &fileName)
{
    std::ofstream outFile(fileName);
    if (!outFile) {
        std::cerr << "Error: Could not open file " << fileName << " for writing." << std::endl;
        return;
    }

    for (int num : buffer) {
        outFile << num << " ";
    }
    outFile << std::endl;
}

void MultiPhaseSorter::multiPhaseMerge(const std::vector<std::string> &tempFiles,
                                       const std::string &outputFile)
{
    const int n = tempFiles.size();

    std::vector<std::ifstream> fileStreams;
    for (int i = 0; i < n; ++i) {
        fileStreams.emplace_back(tempFiles[i]);
        if (!fileStreams.back().is_open()) {
            std::cerr << "Error: Could not open temp file " << tempFiles[i] << std::endl;
            return;
        }
    }

    std::ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open output file " << outputFile << std::endl;
        return;
    }

    std::vector<int> currentNumbers(n, std::numeric_limits<int>::max());
    std::vector<bool> hasValue(n, false);

    for (int i = 0; i < n; ++i) {
        if (fileStreams[i] >> currentNumbers[i]) {
            hasValue[i] = true;
        }
    }

    while (true) {
        int minNumber = std::numeric_limits<int>::max();
        int minIndex = -1;

        for (int i = 0; i < n; ++i) {
            if (hasValue[i] && currentNumbers[i] < minNumber) {
                minNumber = currentNumbers[i];
                minIndex = i;
            }
        }

        if (minIndex == -1) {
            break;
        }

        outFile << minNumber << " ";

        if (fileStreams[minIndex] >> currentNumbers[minIndex]) {
        } else {
            hasValue[minIndex] = false;
            currentNumbers[minIndex] = std::numeric_limits<int>::max();
        }
    }

    for (auto &stream : fileStreams) {
        stream.close();
    }
    outFile.close();
}

void MultiPhaseSorter::cleanupTempFiles(const std::vector<std::string> &tempFiles)
{
    for (const auto &fileName : tempFiles) {
        std::remove(fileName.c_str());
    }
}
