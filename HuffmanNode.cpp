#include "HuffmanNode.h"

HuffmanNode::HuffmanNode()
{}

int HuffmanNode::frequency() const
{
    return _frequency;
}

void HuffmanNode::setFrequency(int newFrequency)
{
    _frequency = newFrequency;
}

char HuffmanNode::symbol() const
{
    return _symbol;
}

void HuffmanNode::setSymbol(char newSymbol)
{
    _symbol = newSymbol;
}
