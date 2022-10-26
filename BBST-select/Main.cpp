/*
* Author: Tim Klimpel
* Assignment Title: BBST Select
* Assignment Description: 
* Due Date: 6/17/2022
* Date Created: 6/13/2022
* Date Last Modified: 6/19/2022
*/

/*
* Input:
*		- Line 1: input of values to create BBST out of
*   - Line 2: a single number, k
* Process:
*   - Utilize GfG AVL code to build an AVL tree
*   - Utilizing an in-order traversal, step through the tree
*   - use a global count to increment at each node visitied
*   - When the requested k'th node is visitied, report to the user the found value
*		
* Output:
*   - The kth ordered statistic
*
* Assumptions:
*   - AVL tree being used does not support duplicates, so it is asssumed the entry will be all uniqe integer values
*
*/

/*Resources*/
// GFG provided code for AVL tree being used for this process.  
// No modifications made to original code base, only additions of inOrder, Select, and input parsing fucntions
// Link: https://www.geeksforgeeks.org/avl-tree-set-1-insertion/?ref=leftbar-rightbar
 
/*includes*/
#include <iostream>
#include <string>
#include <vector>

#include <Bits.h>

/*Namespaces*/
using namespace std;

/*Constants*/

/*Struct*/

/*Classes*/
// An AVL tree node(from gfg)
class Node
{
public:
  int key;
  Node* left;
  Node* right;
  int height;
};

/*Function Declerations*/

//gfg code
int maxTwoInt(int a, int b);
int height(Node* N);
Node* newNode(int key);
Node* rightRotate(Node* y);
Node* leftRotate(Node* x);
int getBalance(Node* N);
Node* insert(Node* node, int key);
void preOrder(Node* root);
//end gfg code

/*
* Description: inOrder prints the in-order traversal of a given tree
* return: None, all printed to terminal
* precondition: tree is complete and traversable.  A BST will produce a sorted output with this function
* postcondition: The in-order traversal is printed to the terminal.
*/
void inOrder(Node* root);

/*
* Description: select will print the requested k'th ordered statistic of the tree
* return: None, the k'th ordered statistic is printed to the terminal only
* precondition: The root passed is to a BST. the kth passed is greater than 0 and count is set to 0 before starting function
* postcondition: The kth ordered statistic is printed to the console, being obtained by an in-order traversal of the tree. Count has been modified
*   by the function, and requires to be reset if the function is called again.
*/
void select(Node* root, int kth, int& count);

/*
* Description: splitString will split an input string that is delimited by the given char value
* return: a vector<int> of the split string for use in creating a AVL tree by the program
* precondition: string s and char delimit are valid for splitting into a vector of integers using the delimit char
* postcondition: The correct vecotr is returned by the function.  the input string is un-modified.
*/
vector<int> splitString(string s, char delimit);

/*************
* MAIN START *
*************/
int main()
{
  Node* root = NULL;
  int count = 0;
  int kVal;
  vector<int> rootVals;
  string tempInput;

  cout << "Enter list of nodes to create...\n";
  getline(cin, tempInput);
  rootVals = splitString(tempInput, ' ');

  cout << "Enter the kth ordered statistic you want to see of the above numbers... ";
  cin >> kVal;

  for (int i = 0; i < rootVals.size(); i++) {
    root = insert(root, rootVals[i]);
  }

  cout << "InOrder traversal:\n"; //Should be sorted order of ints, use for debugging
  inOrder(root);

  cout << endl;

  cout << "kth of " << kVal << ":\n";
  select(root, kVal, count);

  return 0;
}
/*************
* MAIN END   *
*************/

/*Function definitions*/

int maxTwoInt(int a, int b)
{
  return (a > b) ? a : b;
}

int height(Node* N)
{
  if (N == NULL)
    return 0;
  return N->height;
}

Node* newNode(int key)
{
  Node* node = new Node();
  node->key = key;
  node->left = NULL;
  node->right = NULL;
  node->height = 1; // new node is initially add at leaf
  return(node);
}

Node* rightRotate(Node* y)
{
  Node* x = y->left;
  Node* T2 = x->right;

  // Perform rotation
  x->right = y;
  y->left = T2;

  // Update heights
  y->height = max(height(y->left),
    height(y->right)) + 1;
  x->height = max(height(x->left),
    height(x->right)) + 1;

  // Return new root
  return x;
}

Node* leftRotate(Node* x)
{
  Node* y = x->right;
  Node* T2 = y->left;

  // Perform rotation
  y->left = x;
  x->right = T2;

  // Update heights
  x->height = max(height(x->left),
    height(x->right)) + 1;
  y->height = max(height(y->left),
    height(y->right)) + 1;

  // Return new root
  return y;
}

int getBalance(Node* N)
{
  if (N == NULL)
    return 0;
  return height(N->left) - height(N->right);
}

Node* insert(Node* node, int key)
{
  /* 1. Perform the normal BST insertion */
  if (node == NULL)
    return(newNode(key));

  if (key < node->key)
    node->left = insert(node->left, key);
  else if (key > node->key)
    node->right = insert(node->right, key);
  else // Equal keys are not allowed in BST
    return node;

  /* 2. Update height of this ancestor node */
  node->height = 1 + max(height(node->left),
    height(node->right));

  /* 3. Get the balance factor of this ancestor
    node to check whether this node became
    unbalanced */
  int balance = getBalance(node);

  // If this node becomes unbalanced, then
  // there are 4 cases

  // Left Left Case
  if (balance > 1 && key < node->left->key)
    return rightRotate(node);

  // Right Right Case
  if (balance < -1 && key > node->right->key)
    return leftRotate(node);

  // Left Right Case
  if (balance > 1 && key > node->left->key)
  {
    node->left = leftRotate(node->left);
    return rightRotate(node);
  }

  // Right Left Case
  if (balance < -1 && key < node->right->key)
  {
    node->right = rightRotate(node->right);
    return leftRotate(node);
  }

  /* return the (unchanged) node pointer */
  return node;
}

void preOrder(Node* root)
{
  if (root != NULL)
  {
    cout << root->key << " ";
    preOrder(root->left);
    preOrder(root->right);
  }
}

void inOrder(Node* root)
{
  if (root != NULL)
  {
    inOrder(root->left);
    cout << root->key << " ";
    inOrder(root->right);
  }
}

void select(Node* root, int kth, int& count)
{

  if (root != NULL)
  {
    select(root->left, kth, count);
    count++;
    if (count == kth) {
      cout << "The " << kth << " ordered stat is... " << root->key << endl;
    }
    select(root->right, kth, count);
  }

}

vector<int> splitString(string s, char delimit)
{
  vector<int> temp;
  string tempS = "";

  for (int i = 0; i < s.length(); i++) {
    if (s[i] == delimit) {
      temp.push_back(stoi(tempS));
      tempS = "";
    }
    else {
      tempS += s[i];
    }
  }
  temp.push_back(stoi(tempS));

  return temp;
}
