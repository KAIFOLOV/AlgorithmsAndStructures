#ifndef BINARYTREE_H
#define BINARYTREE_H

#include "treenode.h"

class BinaryTree
{
public:
    BinaryTree(TreeNode *root = nullptr);
    BinaryTree(const BinaryTree &other); // Конструктор копирования
    ~BinaryTree(); // Деструктор

    // Основные методы
    TreeNode *getRoot() const;
    void setRoot(TreeNode *newRoot);

    void clear();
    bool isEmpty() const;

    bool addNode(const double &value); // Добавляет узел с заданным значением
    bool deleteNode(const double &value); // Удаляет узел по ключу
    TreeNode *findNode(const double &value) const; // Поиск узла по значению

    int getHeight() const;
    int getNodeCount() const;
    double getMinKey() const;
    double getMaxKey() const;

    bool isBalanced() const;
    int getLevel(double value) const;

    std::vector<int> getSortedKeys() const;

    void printTreeHorizontally() const;
    void printTreeByLevels() const;
    void printLeaves() const;

    // Оператор присваивания
    BinaryTree &operator=(const BinaryTree &other);

private:
    TreeNode *_root;

    // Вспомогательные функции
    void deleteSubtree(TreeNode *node);
    TreeNode *copySubtree(TreeNode *node) const;
    int calculateHeight(TreeNode *node) const;
    int countNodes(TreeNode *node) const;
    bool checkBalance(TreeNode *node) const;
    void inOrderTraversal(TreeNode *node, std::vector<int> &keys) const;
    void printHorizontal(TreeNode *node, int space) const;
};
#endif // BINARYTREE_H
