#pragma once

struct Node
{
  int val;
  Node* left;
  Node* right;
  Node* parent;
};

class BST
{
public:
  Node createNode(int val);
  void insertNode(Node node, Node *root);
};

