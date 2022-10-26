#include "Node.h"

Node::Node()
{
  aPrivate = -99;
  bPrivate = -99;
}

int Node::getA()
{
  return aPrivate;
}

int Node::getB()
{
  return bPrivate;
}

void Node::setA(int setInt)
{
  aPrivate = setInt;
}

void Node::setB(int setInt)
{
  bPrivate = setInt;
}

int Node::sum()
{
  return aPrivate + bPrivate;
}
