#include "multiphasesorter.h"

#include <fstream>
#include <random>

#include "binarytree.h"

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

void testTree()
{
    BinaryTree *tree = new BinaryTree();

    tree->addNode(10);
    tree->addNode(5);
    tree->addNode(15);
    tree->addNode(2);
    tree->addNode(7);
    tree->addNode(12);
    tree->addNode(18);

    std::cout << "Initial tree (Horizontal view):\n";
    tree->printTreeHorizontally();

    std::cout << "\nHeight Tree: " << tree->getHeight() << std::endl;
    std::cout << "Count nodes: " << tree->getNodeCount() << std::endl;

    std::cout << "Minimal value: " << tree->getMinKey() << std::endl;
    std::cout << "Maximal value: " << tree->getMaxKey() << std::endl;

    if (tree->isBalanced()) {
        std::cout << "Tree balanced." << std::endl;
    } else {
        std::cout << "Tree not balanced." << std::endl;
    }

    std::vector<int> sortedKeys = tree->getSortedKeys();
    std::cout << "Values tree ascending order : ";
    for (int key : sortedKeys) {
        std::cout << key << " ";
    }
    std::cout << std::endl;

    std::cout << "Leaves: ";
    tree->printLeaves();

    std::cout << "\nDelete node with value = 7." << std::endl;
    if (tree->deleteNode(7)) {
        std::cout << "Node 7 delete." << std::endl;
    } else {
        std::cout << "Node 7 not found." << std::endl;
    }

    std::cout << "Tree after delete 7:\n";
    tree->printTreeHorizontally();
}

int main()
{
    const std::string fileName = "kek.txt";
    const int numbersCount = 1000;
    const int maxNumberValue = 10000;

    for (int i = 1; i < 12; i++) {
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

    testTree();

    return 0;
}
