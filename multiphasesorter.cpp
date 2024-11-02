#include "multiphasesorter.h"
#include <algorithm>
#include <fstream>
#include <iostream>

void MultiPhaseSorter::sort(const std::string &inputFile, const int numFiles)
{
    _numFiles = numFiles;

    initialize();

    if (!splitData(inputFile))
        return;
    merge();

    writeToOutput(inputFile);
    cleanupTempFiles();
}

void MultiPhaseSorter::initialize()
{
    createAuxiliaryFiles();

    _L = 1;
    _ip = std::vector<int>(_numFiles - 1, 1);
    _ms = std::vector<int>(_numFiles - 1, 1);
    _ip.push_back(0);
    _ms.push_back(0);
}

void MultiPhaseSorter::createAuxiliaryFiles()
{
    for (int i = 0; i < _numFiles; ++i) {
        _tempFiles.push_back("temp" + std::to_string(i) + ".txt");
    }
}

bool MultiPhaseSorter::splitData(const std::string &inputFile)
{
    std::ifstream inFile(inputFile);
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open input file.\n";
        return false;
    }

    inFile.seekg(0, std::ios::end);
    if (inFile.tellg() == 0) {
        std::cerr << "Error: Input file is empty.\n";
        inFile.close();
        return false;
    }
    inFile.seekg(0, std::ios::beg);

    std::vector<std::ofstream> tempFileStreams(_numFiles - 1);
    for (int i = 0; i < _numFiles - 1; ++i) {
        tempFileStreams[i].open(_tempFiles[i]);

        if (!tempFileStreams[i].is_open()) {
            for (int j = 0; j < i; ++j) {
                tempFileStreams[j].close();
            }
            inFile.close();
            return false;
        }
    }

    int i = 0;
    int currentNumber, lastNumber;
    inFile >> currentNumber;

    while (true) {
        do {
            lastNumber = currentNumber;

            tempFileStreams[i] << lastNumber << " ";
        } while (inFile >> currentNumber && lastNumber < currentNumber);

        tempFileStreams[i] << _SEGMENT_DELIMITER << " ";
        _ms[i]--;

        if (inFile.eof()) {
            break;
        }

        if (_ms[i] < _ms[i + 1]) {
            i++;
        } else if (_ms[i] == 0) {
            recalculateLevels();
            i = 0;
        } else {
            i = 0;
        }
    }

    for (auto &fileStream : tempFileStreams) {
        fileStream.close();
    }

    inFile.close();
    return true;
}

void MultiPhaseSorter::rotateFiles(std::vector<std::fstream> &files)
{
    files[_numFiles - 2].close();
    files[_numFiles - 1].close();

    files[_numFiles - 2].open(_tempFiles[_numFiles - 2], std::fstream::out);
    files[_numFiles - 1].open(_tempFiles[_numFiles - 1], std::fstream::in);

    std::string lastTempFile = _tempFiles.back();
    int lastIp = _ip.back();
    int lastMs = _ms.back();

    for (int i = _numFiles - 1; i > 0; i--) {
        _tempFiles[i] = _tempFiles[i - 1];
        _ip[i] = _ip[i - 1];
        _ms[i] = _ms[i - 1];
    }

    _tempFiles[0] = lastTempFile;
    _ip[0] = lastIp;
    _ms[0] = lastMs;
}

void MultiPhaseSorter::merge()
{
    std::vector<std::fstream *> tempFiles(_numFiles);

    for (int i = 0; i < _numFiles - 1; ++i) {
        tempFiles[i] = new std::fstream(_tempFiles[i], std::ios::in);
        if (!tempFiles[i]->is_open()) {
            std::cerr << "Error: Could not open temp file " << _tempFiles[i] << " for reading.\n";
            for (int j = 0; j <= i; ++j) {
                delete tempFiles[j];
            }
            return;
        }
    }

    tempFiles[_numFiles - 1] = new std::fstream(_tempFiles[_numFiles - 1], std::fstream::out);

    while (_L > 0) {
        int min = *std::min_element(_ms.begin(), _ms.end());

        if (min > 0) {
            for (int j = 0; j < _numFiles - 1; j++) {
                _ip[j]--;
                _ms[j]--;
            }
            _ms.back()++;
        }

        mergeFile(tempFiles);
        --_L;

        tempFiles[_numFiles - 2]->close();
        tempFiles[_numFiles - 1]->close();

        tempFiles[_numFiles - 2]->open(_tempFiles[_numFiles - 2], std::fstream::out);
        tempFiles[_numFiles - 1]->open(_tempFiles[_numFiles - 1], std::fstream::in);

        std::string name_tmp = _tempFiles[_numFiles - 1];
        std::fstream *file_tmp = tempFiles[_numFiles - 1];

        int ip_tmp = _ip[_numFiles - 1];
        int ms_tmp = _ms[_numFiles - 1];

        for (int i = _numFiles - 1; i > 0; i--) {
            _tempFiles[i] = _tempFiles[i - 1];
            tempFiles[i] = tempFiles[i - 1];

            _ip[i] = _ip[i - 1];
            _ms[i] = _ms[i - 1];
        }

        _tempFiles[0] = name_tmp;
        tempFiles[0] = file_tmp;

        _ip[0] = ip_tmp;
        _ms[0] = ms_tmp;
    }

    for (auto file : tempFiles) {
        file->close();
        delete file;
    }
}

void MultiPhaseSorter::mergeFile(std::vector<std::fstream *> &files)
{
    std::vector<int> segment(_numFiles - 1, _SEGMENT_DELIMITER);
    int eos;

    while (_ip[_numFiles - 2] > 0) {
        eos = 0;

        bool hasRealData = false;

        for (int i = 0; i < _numFiles - 1; i++) {
            if (_ms[i] > 0) {
                eos++;

                _ip[i]--;
                _ms[i]--;
            } else {
                hasRealData = true;
                if (_ip[i] <= 0)
                    continue;
                *files[i] >> segment[i];
            }
        }

        while (eos < _numFiles - 1) {
            int minIndex = min(segment);

            *files[_numFiles - 1] << segment[minIndex] << " ";

            *files[minIndex] >> segment[minIndex];

            if (segment[minIndex] == _SEGMENT_DELIMITER) {
                eos++;

                _ip[minIndex]--;
            }
        }

        if (!hasRealData) {
            _ms[_numFiles - 1]++;
            _ip[_numFiles - 1]++;
        } else {
            _ip[_numFiles - 1]++;
            *files[_numFiles - 1] << _SEGMENT_DELIMITER << " ";
        }
    }
}

int MultiPhaseSorter::min(std::vector<int> &segment)
{
    int minIndex = 0;
    int minValue = std::numeric_limits<int>::max();

    for (int i = 0; i < _numFiles - 1; i++) {
        if (minValue > segment[i] && segment[i] != _SEGMENT_DELIMITER) {
            minValue = segment[i];
            minIndex = i;
        }
    }

    return minIndex;
}

void MultiPhaseSorter::cleanupTempFiles()
{
    for (const auto &fileName : _tempFiles) {
        std::remove(fileName.c_str());
    }
    _tempFiles.clear();
}

void MultiPhaseSorter::recalculateLevels()
{
    _L++;
    int ip0 = _ip[0];

    for (int k = 0; k < _numFiles - 1; ++k) {
        _ms[k] = _ip[k + 1] - _ip[k] + ip0;
        _ip[k] = _ip[k + 1] + ip0;
    }
}

void MultiPhaseSorter::writeToOutput(const std::string &inputFile)
{
    std::ifstream sortedFile(_tempFiles.front());
    std::ofstream outputFile(inputFile, std::ios::trunc);

    if (!sortedFile.is_open() || !outputFile.is_open()) {
        std::cerr << "Error: Could not open files for final writing.\n";
        return;
    }

    int a;

    sortedFile >> a;

    while (a != _SEGMENT_DELIMITER) {
        outputFile << a << " ";

        sortedFile >> a;
    }

    sortedFile.close();
    outputFile.close();
}
