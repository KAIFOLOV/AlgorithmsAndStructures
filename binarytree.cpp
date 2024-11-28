#include "binarytree.h"

#include "treenode.h"

#include <iostream>
#include <queue>
#include <algorithm>
#include <stdexcept>

// Реализация BinaryTree
BinaryTree::BinaryTree() : _root(nullptr)
{}

BinaryTree::BinaryTree(const BinaryTree &other) : _root(copySubtree(other._root))
{}

BinaryTree::~BinaryTree()
{
    clear();
}

TreeNode *BinaryTree::getRoot() const
{
    return _root;
}

void BinaryTree::clear()
{
    deleteSubtree(_root);
    delete _root;
    _root = nullptr;
}
bool BinaryTree::isEmpty() const
{
    return _root == nullptr;
}

TreeNode *BinaryTree::findNode(const double &value) const
{
    return findNodeRecursive(_root, value);
}

bool BinaryTree::addNode(const double &value)
{
    if (!_root) {
        _root = new TreeNode();
        _root->setValue(value);
        return true;
    }
    return addNodeRecursive(_root, value);
}

bool BinaryTree::deleteNode(const double &value)
{
    TreeNode *node = findNode(value);
    if (!node)
        return false;

    if (!node->getLeftChild() && !node->getRightChild()) {
        if (node == _root)
            _root = nullptr;

        else if (node->getParent()->getLeftChild() == node)
            node->getParent()->setLeftChild(nullptr);
        else
            node->getParent()->setRightChild(nullptr);

    } else if (node->getLeftChild() && node->getRightChild()) {
        TreeNode *successor = findMin(node->getRightChild());
        node->setValue(successor->getValue());
        return deleteNode(successor->getValue());

    } else {
        TreeNode *child = node->getLeftChild() ? node->getLeftChild() : node->getRightChild();
        if (node == _root)
            _root = child;
        else if (node->getParent()->getLeftChild() == node)
            node->getParent()->setLeftChild(child);
        else
            node->getParent()->setRightChild(child);
        child->setParent(node->getParent());
    }
    delete node;
    return true;
}

int BinaryTree::getHeight() const
{
    return calculateHeight(_root);
}
int BinaryTree::getNodeCount() const
{
    return countNodes(_root);
}

int BinaryTree::getMinKey() const
{
    if (!_root)
        throw std::runtime_error("Tree is empty");
    return findMin(_root)->getValue();
}

int BinaryTree::getMaxKey() const
{
    if (!_root)
        throw std::runtime_error("Tree is empty");
    return findMax(_root)->getValue();
}

bool BinaryTree::isBalanced() const
{
    return checkBalance(_root);
}

int BinaryTree::getLevel(int value) const
{
    return findLevel(_root, value, 0);
}

std::vector<int> BinaryTree::getSortedKeys() const
{
    std::vector<int> keys;
    inOrderTraversal(_root, keys);
    return keys;
}

void BinaryTree::printTreeHorizontally() const
{
    printHorizontal(_root, 0);
}

void BinaryTree::printTreeByLevels() const
{
    if (!_root)
        return;
    std::queue<TreeNode *> q;
    q.push(_root);
    while (!q.empty()) {
        TreeNode *node = q.front();
        q.pop();
        std::cout << node->getValue() << " ";
        if (node->getLeftChild())
            q.push(node->getLeftChild());
        if (node->getRightChild())
            q.push(node->getRightChild());
    }
    std::cout << std::endl;
}

void BinaryTree::printLeaves() const
{
    printLeavesRecursive(_root);
}

TreeNode *BinaryTree::findNodeRecursive(TreeNode *node, int value) const
{
    if (!node || node->getValue() == value)
        return node;
    if (value < node->getValue())
        return findNodeRecursive(node->getLeftChild(), value);
    return findNodeRecursive(node->getRightChild(), value);
}

bool BinaryTree::addNodeRecursive(TreeNode *node, const double &value)
{
    if (value < node->getValue()) {
        if (node->getLeftChild())
            return addNodeRecursive(node->getLeftChild(), value);

        auto tmpNode = new TreeNode(node);
        tmpNode->setValue(value);
        node->setLeftChild(tmpNode);
        return true;
    } else {
        if (node->getRightChild())
            return addNodeRecursive(node->getRightChild(), value);

        auto tmpNode = new TreeNode(node);
        tmpNode->setValue(value);
        node->setRightChild(tmpNode);
        return true;
    }
}

void BinaryTree::deleteSubtree(TreeNode *node)
{
    if (node) {
        delete node->getLeftChild();
        delete node->getRightChild();
    }
}

TreeNode *BinaryTree::copySubtree(TreeNode *node) const
{
    if (!node)
        return nullptr;
    TreeNode *copy = new TreeNode();
    copy->setValue(node->getValue());
    copy->setLeftChild(copySubtree(node->getLeftChild()));
    copy->setRightChild(copySubtree(node->getRightChild()));
    return copy;
}

int BinaryTree::calculateHeight(TreeNode *node) const
{
    if (!node)
        return 0;
    return 1
           + std::max(calculateHeight(node->getLeftChild()),
                      calculateHeight(node->getRightChild()));
}

int BinaryTree::countNodes(TreeNode *node) const
{
    if (!node)
        return 0;
    return 1 + countNodes(node->getLeftChild()) + countNodes(node->getRightChild());
}

TreeNode *BinaryTree::findMin(TreeNode *node) const
{
    while (node && node->getLeftChild()) node = node->getLeftChild();
    return node;
}

TreeNode *BinaryTree::findMax(TreeNode *node) const
{
    while (node && node->getRightChild()) node = node->getRightChild();
    return node;
}

bool BinaryTree::checkBalance(TreeNode *node) const
{
    if (!node)
        return true;
    int leftHeight = calculateHeight(node->getLeftChild());
    int rightHeight = calculateHeight(node->getRightChild());
    return std::abs(leftHeight - rightHeight) <= 1 && checkBalance(node->getLeftChild())
           && checkBalance(node->getRightChild());
}

int BinaryTree::findLevel(TreeNode *node, int value, int level) const
{
    if (!node)
        return -1;
    if (node->getValue() == value)
        return level;

    int leftLevel = findLevel(node->getLeftChild(), value, level + 1);
    if (leftLevel != -1)
        return leftLevel;

    return findLevel(node->getRightChild(), value, level + 1);
}

void BinaryTree::inOrderTraversal(TreeNode *node, std::vector<int> &keys) const
{
    if (!node)
        return;

    inOrderTraversal(node->getLeftChild(), keys);
    keys.push_back(node->getValue());
    inOrderTraversal(node->getRightChild(), keys);
}

void BinaryTree::printHorizontal(TreeNode *node, int space) const
{
    const int INDENT = 4;

    if (!node)
        return;

    space += INDENT;

    printHorizontal(node->getRightChild(), space);

    std::cout << std::endl;
    for (int i = INDENT; i < space; i++) std::cout << " ";
    std::cout << node->getValue() << std::endl;

    printHorizontal(node->getLeftChild(), space);
}

void BinaryTree::printLeavesRecursive(TreeNode *node) const
{
    if (!node)
        return;

    if (!node->getLeftChild() && !node->getRightChild()) {
        std::cout << node->getValue() << " ";
        return;
    }

    printLeavesRecursive(node->getLeftChild());
    printLeavesRecursive(node->getRightChild());
}
