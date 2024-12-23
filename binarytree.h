#ifndef BINARYTREE_H
#define BINARYTREE_H

#include "treenode.h"

class BinaryTree
{
public:
    BinaryTree();
    BinaryTree(const BinaryTree &other);
    ~BinaryTree();

    TreeNode *getRoot() const;

    void clear();
    bool isEmpty() const;

    TreeNode *findNode(const double &value) const;
    bool addNode(const double &value);
    bool deleteNode(const double &value);

    int getHeight() const;
    int getNodeCount() const;

    int getMinKey() const;
    int getMaxKey() const;

    bool isBalanced() const;

    int getLevel(int value) const;

    std::vector<int> getSortedKeys() const;

    void printTreeHorizontally() const;
    void printTreeByLevels() const;
    void printLeaves() const;

    BinaryTree &operator=(const BinaryTree &other);

private:
    TreeNode *_root = nullptr;

    TreeNode *findNodeRecursive(TreeNode *node, int value) const;
    bool addNodeRecursive(TreeNode *node, const double &value);

    void deleteSubtree(TreeNode *node);
    TreeNode *copySubtree(TreeNode *node) const;

    int calculateHeight(TreeNode *node) const;
    int countNodes(TreeNode *node) const;

    TreeNode *findMin(TreeNode *node) const;
    TreeNode *findMax(TreeNode *node) const;

    bool checkBalance(TreeNode *node) const;

    int findLevel(TreeNode *node, int value, int level) const;

    void inOrderTraversal(TreeNode *node, std::vector<int> &keys) const;

    void printHorizontal(TreeNode *node, int space) const;
    void printLeavesRecursive(TreeNode *node) const;

    bool deleteNode(TreeNode *node);
};

#endif // BINARYTREE_H
