#include "binarytree.h"
#include <cstdlib>
#include <iostream>
#include <queue>

BinaryTree::BinaryTree(TreeNode *root) : _root(root)
{}

BinaryTree::BinaryTree(const BinaryTree &other)
{
    _root = copySubtree(other._root);
}

TreeNode *BinaryTree::getRoot() const
{
    return _root;
}

void BinaryTree::setRoot(TreeNode *newRoot)
{
    _root = newRoot;
}

BinaryTree::~BinaryTree()
{
    clear();
}

void BinaryTree::clear()
{
    deleteSubtree(_root);
    _root = nullptr;
}

bool BinaryTree::isEmpty() const
{
    return _root == nullptr;
}

bool BinaryTree::addNode(const double &value)
{
    TreeNode *newNode = new TreeNode();
    newNode->setValue(value);

    if (isEmpty()) {
        _root = newNode;
        return true;
    }

    TreeNode *current = _root;
    while (true) {
        int direction = rand() % 2;
        TreeNode *child = current->getChild(direction);

        if (!child) {
            current->addChild(newNode);
            newNode->setParent(current);
            return true;
        }
        current = child;
    }
}

bool BinaryTree::deleteNode(const double &value)
{
    TreeNode *node = findNode(value);
    if (!node)
        return false;

    if (node->getParent()) {
        node->getParent()->removeChild(node);
    } else {
        _root = nullptr;
    }

    deleteSubtree(node);
    return true;
}

TreeNode *BinaryTree::findNode(const double &value) const
{
    std::queue<TreeNode *> nodes;
    nodes.push(_root);

    while (!nodes.empty()) {
        TreeNode *node = nodes.front();
        nodes.pop();

        if (node->getValue() == value)
            return node;

        for (TreeNode *child : node->getChildren()) nodes.push(child);
    }
    return nullptr;
}

int BinaryTree::getHeight() const
{
    return calculateHeight(_root);
}

int BinaryTree::getNodeCount() const
{
    return countNodes(_root);
}

double BinaryTree::getMinKey() const
{
    double minKey = _root ? _root->getValue() : 0;
    std::queue<TreeNode *> nodes;
    nodes.push(_root);

    while (!nodes.empty()) {
        TreeNode *node = nodes.front();
        nodes.pop();
        minKey = std::min(minKey, node->getValue());

        for (TreeNode *child : node->getChildren()) nodes.push(child);
    }
    return minKey;
}

double BinaryTree::getMaxKey() const
{
    double maxKey = _root ? _root->getValue() : 0;
    std::queue<TreeNode *> nodes;
    nodes.push(_root);

    while (!nodes.empty()) {
        TreeNode *node = nodes.front();
        nodes.pop();
        maxKey = std::max(maxKey, node->getValue());

        for (TreeNode *child : node->getChildren()) nodes.push(child);
    }
    return maxKey;
}

bool BinaryTree::isBalanced() const
{
    return checkBalance(_root);
}

int BinaryTree::getLevel(double value) const
{
    std::queue<std::pair<TreeNode *, int>> nodes;
    nodes.push({ _root, 0 });

    while (!nodes.empty()) {
        TreeNode *node = nodes.front().first;
        int level = nodes.front().second;
        nodes.pop();

        if (node->getValue() == value)
            return level;

        for (TreeNode *child : node->getChildren()) nodes.push({ child, level + 1 });
    }
    return -1;
}

std::vector<int> BinaryTree::getSortedKeys() const
{
    std::vector<int> keys;
    inorderTraversal(_root, keys);
    return keys;
}

void BinaryTree::printTreeHorizontally() const
{
    printHorizontal(_root, 0);
}

void BinaryTree::printTreeByLevels() const
{
    if (isEmpty())
        return;

    std::queue<TreeNode *> nodes;
    nodes.push(_root);

    while (!nodes.empty()) {
        int levelSize = nodes.size();

        for (int i = 0; i < levelSize; ++i) {
            TreeNode *node = nodes.front();
            nodes.pop();
            std::cout << node->getValue() << " ";

            for (TreeNode *child : node->getChildren()) nodes.push(child);
        }
        std::cout << std::endl;
    }
}

void BinaryTree::printLeaves() const
{
    if (isEmpty())
        return;

    std::queue<TreeNode *> nodes;
    nodes.push(_root);

    while (!nodes.empty()) {
        TreeNode *node = nodes.front();
        nodes.pop();

        if (node->childCount() == 0)
            std::cout << node->getValue() << " ";

        for (TreeNode *child : node->getChildren()) nodes.push(child);
    }
    std::cout << std::endl;
}

BinaryTree &BinaryTree::operator=(const BinaryTree &other)
{
    if (this != &other) {
        clear();
        _root = copySubtree(other._root);
    }
    return *this;
}

void BinaryTree::deleteSubtree(TreeNode *node)
{
    if (!node)
        return;
    for (TreeNode *child : node->getChildren()) {
        deleteSubtree(child);
    }
    delete node;
}

TreeNode *BinaryTree::copySubtree(TreeNode *node) const
{
    if (!node)
        return nullptr;
    TreeNode *newNode = new TreeNode(*node);
    for (TreeNode *child : node->getChildren()) {
        newNode->addChild(copySubtree(child));
    }
    return newNode;
}

int BinaryTree::calculateHeight(TreeNode *node) const
{
    if (!node)
        return 0;
    int height = 0;
    for (TreeNode *child : node->getChildren()) {
        height = std::max(height, calculateHeight(child));
    }
    return height + 1;
}

int BinaryTree::countNodes(TreeNode *node) const
{
    if (!node)
        return 0;
    int count = 1;
    for (TreeNode *child : node->getChildren()) {
        count += countNodes(child);
    }
    return count;
}

bool BinaryTree::checkBalance(TreeNode *node) const
{
    if (!node)
        return true;
    int leftHeight = calculateHeight(node->getChild(0));
    int rightHeight = calculateHeight(node->getChild(1));
    return std::abs(leftHeight - rightHeight) <= 1 && checkBalance(node->getChild(0))
           && checkBalance(node->getChild(1));
}

void BinaryTree::inorderTraversal(TreeNode *node, std::vector<int> &keys) const
{
    if (!node)
        return;
    if (node->getChild(0))
        inorderTraversal(node->getChild(0), keys);
    keys.push_back(node->getValue());
    if (node->getChild(1))
        inorderTraversal(node->getChild(1), keys);
}

void BinaryTree::printHorizontal(TreeNode *node, int space) const
{
    if (!node)
        return;

    const int SPACING = 5;
    space += SPACING;

    if (node->getChild(1))
        printHorizontal(node->getChild(1), space);

    std::cout << std::endl;
    for (int i = SPACING; i < space; i++) std::cout << " ";
    std::cout << node->getValue() << "\n";

    if (node->getChild(0))
        printHorizontal(node->getChild(0), space);
}
