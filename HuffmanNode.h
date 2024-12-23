#ifndef HUFFMANNODE_H
#define HUFFMANNODE_H

#include "treenode.h"

class HuffmanNode : public TreeNode
{
public:
    HuffmanNode();

    int frequency() const;
    void setFrequency(int newFrequency);

    char symbol() const;
    void setSymbol(char newSymbol);

private:
    char _symbol;
    int _frequency;
};

#endif // HUFFMANNODE_H
