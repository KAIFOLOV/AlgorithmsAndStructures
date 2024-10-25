#include "multiphasesorter.h"

#include <fstream>
#include <random>

/// Генерирует файл с рандомными значениями в диапазоне от -maxNumberValue до maxNumberValue
bool createFileWithRandomNumbers(const std::string &fileName,
                                 const int numbersCount,
                                 const int maxNumberValue)
{
    std::ofstream outFile(fileName);

    if (!outFile) {
        std::cerr << "Error: Could not open file " << fileName << " for writing.\n";
        return false;
    }

    std::mt19937 rand(::time(0));

    for (int i = 0; i < numbersCount; ++i) {
        int randomNumber = std::rand() % (2 * maxNumberValue + 1) - maxNumberValue;
        outFile << randomNumber << " ";
    }

    outFile.close();

    if (!outFile) {
        std::cerr << "Error: Failed to write to file " << fileName << ".\n";
        return false;
    }

    return true;
}

bool isFileContainsSortedArray(const std::string &fileName)
{
    std::ifstream inFile(fileName);
    if (!inFile) {
        std::cerr << "Error: Could not open file " << fileName << " for reading.\n";
        return false;
    }

    int previousNumber;
    int currentNumber;

    inFile >> previousNumber;
    while (inFile >> currentNumber) {
        if (currentNumber < previousNumber) {
            return false;
        }
        previousNumber = currentNumber;
    }
    return true;
}

int createAndSortFile(const std::string &fileName, const int numbersCount, const int maxNumberValue)
{
    if (numbersCount == 0)
        return -1;

    if (maxNumberValue == 0)
        return -2;

    if (!createFileWithRandomNumbers(fileName, numbersCount, maxNumberValue)) {
        return -3;
    }

    MultiPhaseSorter tmp;
    tmp.sort(fileName);

    if (!isFileContainsSortedArray(fileName)) {
        return -4;
    }

    return 1;
}

int main()
{
    const std::string fileName = "kek.txt";
    const int numbersCount = 10;
    const int maxNumberValue = 100;

    createFileWithRandomNumbers("lol.txt", numbersCount, maxNumberValue);

    for (int i = 1; i < 2; i++) {
        switch (createAndSortFile(fileName, numbersCount, maxNumberValue)) {
        case 1:
            std::cout << "Test " << i << " passed." << std::endl;
            break;

        case -1:
            std::cout << "Test " << i << " failed: MaxNumber = 0." << std::endl;
            break;

        case -2:
            std::cout << "Test " << i << " failed: count number = 0." << std::endl;
            break;

        case -3:
            std::cout << "Test " << i << " failed: can't create file." << std::endl;
            break;

        case -4:
            std::cout << "Test " << i << " failed: file isn't sorted." << std::endl;
            break;
        }
    }
    return 0;
}
