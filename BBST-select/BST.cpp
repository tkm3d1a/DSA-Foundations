#include "BST.h"

Node BST::createNode(int val)
{
  Node newNode = Node();

  newNode.val = val;
  newNode.left = nullptr;
  newNode.right = nullptr;
  newNode.parent = nullptr;

  return newNode;
}

void BST::insertNode(Node node, Node *root)
{
  int nodeVal = node.val;

  if (root == nullptr) {
    root = &node;
    return;
  }

  int rootVal = root->val;

  if (nodeVal < rootVal) {
    root->left = &node;
    node.parent = root;
    return;
  }

  if (nodeVal > rootVal) {
    root->right = &node;
    node.parent = root;
    return;
  }
}
