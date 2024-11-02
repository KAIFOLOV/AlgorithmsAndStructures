#include "treenode.h"
#include <algorithm>

TreeNode::TreeNode(TreeNode *parent) : _parent(parent)
{}

TreeNode::TreeNode(const TreeNode &other) : _parent(other._parent)
{}

TreeNode::~TreeNode()
{
    for (auto &child : _children) delete child;
    _children.clear();
}

TreeNode &TreeNode::operator=(const TreeNode &other)
{
    if (this != &other) {
        _parent = other._parent;

        for (TreeNode *child : _children) {
            delete child;
        }
        _children.clear();
        for (TreeNode *child : other._children) {
            TreeNode *clonedChild = new TreeNode(*child);
            clonedChild->setParent(this);
            _children.push_back(clonedChild);
        }
    }
    return *this;
}

double TreeNode::getValue() const
{
    return _value;
}

void TreeNode::setValue(double newValue)
{
    _value = newValue;
}

TreeNode *TreeNode::getChild(int index)
{
    if (index < _children.size())
        return _children[index];

    return nullptr;
}

std::vector<TreeNode *> TreeNode::getChildren() const
{
    return _children;
}

int TreeNode::indexOfChild(TreeNode *node)
{
    auto it = std::find(_children.begin(), _children.end(), node);
    if (it != _children.end()) {
        return std::distance(_children.begin(), it);
    }
    return -1;
}

void TreeNode::addChild(TreeNode *child)
{
    _children.push_back(child);
}

void TreeNode::removeChild(TreeNode *child)
{
    auto ix = indexOfChild(child);
    if (ix >= 0) {
        _children.erase(_children.begin() + ix);
    }
}

int TreeNode::childCount() const
{
    return _children.size();
}

void TreeNode::setParent(TreeNode *parent)
{
    _parent = parent;
}

TreeNode *TreeNode::getParent() const
{
    return _parent;
}
