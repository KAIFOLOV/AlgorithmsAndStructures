#include "multiphasesorter.h"
#include <fstream>
#include <iostream>

void MultiPhaseSorter::sort(const std::string &inputFile, const int numFiles)
{
    std::vector<std::string> tempFiles;
    createAuxiliaryFiles(tempFiles, numFiles);

    splitData(inputFile, tempFiles, numFiles);

    // multiPhaseMerge(tempFiles, inputFile);
    // cleanupTempFiles(tempFiles);
}

void MultiPhaseSorter::createAuxiliaryFiles(std::vector<std::string> &tempFiles, int numFiles)
{
    for (int i = 0; i < numFiles - 1; ++i) {
        tempFiles.push_back("temp" + std::to_string(i) + ".txt");
    }
}

void MultiPhaseSorter::splitData(const std::string &inputFile,
                                 std::vector<std::string> &tempFiles,
                                 int n)
{
    std::ifstream inFile(inputFile);
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open input file.\n";
        return;
    }

    std::vector<std::ofstream> tempFileStreams(n - 1);
    for (int i = 0; i < n - 1; ++i) {
        tempFileStreams[i].open(tempFiles[i]);
        if (!tempFileStreams[i].is_open()) {
            std::cerr << "Error: Could not open temp file " << tempFiles[i] << " for writing.\n";
            return;
        }
    }

    int L = 1;
    std::vector<int> ip(n - 2, 1); // Идеальное распределение
    std::vector<int> ms(n - 2, 1); // Количество отрезков для каждого файла
    ip.push_back(0);
    ms.push_back(0);

    while (true) {
        std::vector<int> currentSegments;
        int i = 0; // Индекс файла

        // Основной цикл разбиения
        while (true) {
            int currentNumber;
            if (!(inFile >> currentNumber)) {
                break;
            }

            // Начинаем формировать фрагмент
            currentSegments.push_back(currentNumber);

            while (inFile >> currentNumber) {
                // Если текущее число больше предыдущего, продолжаем фрагмент
                if (currentNumber < currentSegments.back())
                    break;

                currentSegments.push_back(currentNumber);
            }

            for (auto value : currentSegments) {
                tempFileStreams[i] << value;
            }

            ms[i]--; // Уменьшаем счетчик отрезков для текущего файла

            // Переход к следующему файлу
            if (ms[i] < ms[i + 1] && i < n - 2) {
                i++; // Переходим к следующему файлу
            } else {
                if (ms[i] == 0) {
                    // Если все отрезки в текущем файле закончились, переходим к пересчету //
                    // Переходим на этап 4
                    break;
                } else {
                    i = 0; // Возвращаемся к первому файлу
                }
            }
        }

        // 4. Пересчет уровня и значений в массивах ip и ms
        L++; // Увеличиваем уровень
        int ip0 = ip[0]; // Сохраняем значение ip[0]
        i = 0; // Сбрасываем индекс

        for (int k = 0; k < n - 2; ++k) {
            ms[k] = ip[k + 1] - ip[k] + ip0; // Пересчитываем ms
            ip[k] = ip[k + 1] + ip0; // Пересчитываем ip
        }

        // Проверка на завершение работы
        if (ms[n - 2] == 0) { // Если все отрезки обработаны
            break; // Выходим из основного цикла
        }

        for (int j = 0; j < n - 1; ++j) {
            tempFileStreams[j].close();
            tempFileStreams[j].open(tempFiles[j], std::ios::app);
            if (!tempFileStreams[j].is_open()) {
                std::cerr << "Error: Could not open temp file " << tempFiles[j] << " for writing.\n";
                return;
            }
        }
    }

    // Закрываем все файлы и исходный файл
    for (int i = 0; i < n - 1; ++i) {
        tempFileStreams[i].close();
    }

    inFile.close();
}

void MultiPhaseSorter::cleanupTempFiles(const std::vector<std::string> &tempFiles)
{
    for (const auto &fileName : tempFiles) {
        std::remove(fileName.c_str());
    }
}
