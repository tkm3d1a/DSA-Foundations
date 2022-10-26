#pragma once
class Node
{
private:
  //Variables
  int aPrivate;
  int bPrivate;
  Node *nextNode;
  //Functions

public:
  //Variables
  //Functions
  Node();
  int getA();
  int getB();
  void setA(int setInt);
  void setB(int setInt);
  int sum();
  Node getNextNode();
  void setNextNode(Node* next);
};

