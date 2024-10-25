#include "multiphasesorter.h"
#include <algorithm>
#include <fstream>
#include <iostream>

void MultiPhaseSorter::sort(const std::string &inputFile, const int numFiles)
{
    _numFiles = numFiles;

    initialize();

    splitData(inputFile);
    // mergeData();
}

void MultiPhaseSorter::initialize()
{
    createAuxiliaryFiles();

    _L = 1;
    _ip = std::vector<int>(_numFiles - 2, 1); // Идеальное распределение
    _ms = std::vector<int>(_numFiles - 2, 1); // Количество отрезков для каждого файла
    _ip.push_back(0);
    _ms.push_back(0);
}

void MultiPhaseSorter::createAuxiliaryFiles()
{
    for (int i = 0; i < _numFiles - 1; ++i) {
        _tempFiles.push_back("temp" + std::to_string(i) + ".txt");
    }
}

void MultiPhaseSorter::splitData(const std::string &inputFile)
{
    std::ifstream inFile(inputFile);
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open input file.\n";
        return;
    }

    std::vector<std::ofstream> tempFileStreams(_numFiles - 2);
    for (int i = 0; i < _numFiles - 2; ++i) {
        tempFileStreams[i].open(_tempFiles[i]);
        if (!tempFileStreams[i].is_open()) {
            std::cerr << "Error: Could not open temp file " << _tempFiles[i] << " for writing.\n";
            return;
        }
    }

    bool endOfFileReached = false;

    while (!endOfFileReached) {
        int i = 0;

        int currentNumber;
        int lastNumber;
        if (!(inFile >> currentNumber)) {
            break;
        }

        while (true) {
            tempFileStreams[i] << currentNumber << " ";
            lastNumber = currentNumber;

            while (inFile >> currentNumber) {
                if (currentNumber < lastNumber)
                    break;
                lastNumber = currentNumber;
                tempFileStreams[i] << currentNumber << " ";
            }

            if (inFile.eof()) {
                endOfFileReached = true;
                break;
            }

            _ms[i]--;

            if (_ms[i] < _ms[i + 1] && i < _numFiles - 2) {
                i++;
            } else {
                if (_ms[i] == 0) {
                    recalculateLevels();
                    i = 0;
                } else {
                    i = 0;
                }
            }
        }

        if (_ms[_numFiles - 2] == 0) {
            break;
        }
    }

    // Закрываем все файлы и исходный файл
    for (int i = 0; i < _numFiles - 2; ++i) {
        tempFileStreams[i].close();
    }

    inFile.close();
}

void MultiPhaseSorter::mergeData()
{
    // Открываем файл для записи объединенных данных
    std::ofstream outFileStream(_tempFiles[_numFiles - 1], std::ios::out);
    if (!outFileStream.is_open()) {
        std::cerr << "Error: Could not open temp file for writing output.\n";
        return;
    }

    std::vector<std::ifstream> inFileStreams(_numFiles - 1);
    // Открываем временные файлы для чтения
    for (int i = 0; i < _numFiles - 1; ++i) {
        inFileStreams[i].open(_tempFiles[i]);
        if (!inFileStreams[i].is_open()) {
            std::cerr << "Error: Could not open temp file " << _tempFiles[i] << " for reading.\n";
            return;
        }
    }

    while (_L > 0) {
        // 7.1 Проверяем, все ли ms > 0
        if (std::all_of(_ms.begin(), _ms.end(), [](int x) {
                return x > 0;
            })) {
            for (int &m : _ms) {
                m--; // Уменьшаем фиктивные отрезки
            }
            _ms[_numFiles - 2]++; // Добавляем фиктивный отрезок в выходной файл
        }

        // 7.2 Сливаем текущие отрезки, если ms[i] == 0
        for (int i = 0; i < _numFiles - 1; ++i) {
            if (_ms[i] == 0) {
                int number;
                int lastNumber = INT_MIN;

                // Слияние текущего отрезка
                while (inFileStreams[i] >> number) {
                    if (number < lastNumber) {
                        // Обнаружен конец отрезка
                        break;
                    }
                    outFileStream << number << " ";
                    lastNumber = number;
                }

                // Уменьшаем количество отрезков для текущего файла
                _ms[i]--;
            }
        }

        // 7.3 Проверяем, завершены ли все отрезки
        if (std::all_of(_ms.begin(), _ms.end(), [](int x) {
                return x <= 0;
            })) {
            _L--; // Уменьшаем уровень

            // Переоткрываем файлы для чтения на новом уровне
            for (int i = 0; i < _numFiles - 1; ++i) {
                inFileStreams[i].close();
                inFileStreams[i].clear(); // Сброс флагов состояния потока
                inFileStreams[i].open(_tempFiles[i]);
                if (!inFileStreams[i].is_open()) {
                    std::cerr << "Error: Could not reopen temp file " << _tempFiles[i]
                              << " for reading.\n";
                    return;
                }
            }

            outFileStream.close();
            outFileStream.open(_tempFiles[_numFiles - 1], std::ios::out);
            if (!outFileStream.is_open()) {
                std::cerr << "Error: Could not reopen temp file for writing output.\n";
                return;
            }
        }
    }

    // Завершающая часть: копируем результат в final_result.txt, если достигнут уровень 0
    if (_L == 0) {
        std::ifstream finalFile(_tempFiles[_numFiles - 1]);
        if (finalFile.is_open()) {
            std::ofstream output("final_result.txt");
            output << finalFile.rdbuf();
            finalFile.close();
            output.close();
        }
    }

    // Закрываем все оставшиеся потоки
    for (auto &stream : inFileStreams) {
        if (stream.is_open())
            stream.close();
    }
    if (outFileStream.is_open())
        outFileStream.close();
}

void MultiPhaseSorter::cleanupTempFiles(const std::vector<std::string> &tempFiles)
{
    for (const auto &fileName : tempFiles) {
        std::remove(fileName.c_str());
    }
}

void MultiPhaseSorter::recalculateLevels()
{
    _L++; // Увеличиваем уровень
    int ip0 = _ip[0]; // Сохраняем значение ip[0]

    for (int k = 0; k < _numFiles - 2; ++k) {
        _ms[k] = _ip[k + 1] - _ip[k] + ip0; // Пересчитываем ms
        _ip[k] = _ip[k + 1] + ip0; // Пересчитываем ip
    }
}
