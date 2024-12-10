#include "treenode.h"
#include <algorithm>

TreeNode::TreeNode(TreeNode *parent) : _parent(parent)
{}

TreeNode::TreeNode(const TreeNode &other) : _parent(other._parent)
{}

TreeNode::~TreeNode()
{
    delete _left;
    delete _right;
}

TreeNode &TreeNode::operator=(const TreeNode &other)
{
    if (this != &other) {
        _parent = other._parent;

        delete _left;
        delete _right;

        TreeNode *clonedLeftChild = new TreeNode(*other.getLeftChild());
        TreeNode *clonedRightChild = new TreeNode(*other.getRightChild());

        clonedLeftChild->setParent(this);
        clonedRightChild->setParent(this);

        _left = clonedLeftChild;
        _right = clonedRightChild;
    }
    return *this;
}

double TreeNode::getValue() const
{
    return _value;
}

void TreeNode::setValue(const double &newValue)
{
    _value = newValue;
}

void TreeNode::setLeftChild(TreeNode *left)
{
    _left = left;
}

void TreeNode::setRightChild(TreeNode *right)
{
    _right = right;
}

void TreeNode::setParent(TreeNode *parent)
{
    _parent = parent;
}

TreeNode *TreeNode::getParent() const
{
    return _parent;
}

TreeNode *TreeNode::getLeftChild() const
{
    return _left;
}

TreeNode *TreeNode::getRightChild() const
{
    return _right;
}
