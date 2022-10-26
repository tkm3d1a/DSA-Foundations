#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <unordered_map>
#include <cmath>
#include <iostream>

struct Node {
  Node *left, *right;
  int frequency;
  //int asciiIntCode;
  char data;
  std::string pathInBin;

  Node(char aChar, int charFreq) { //can probably remove constructor.Tried to get too fancy, not worth time?
    left = nullptr;
    right = nullptr;

    data = aChar;
    frequency = charFreq;
  }
};

class Huffman {
private:
  //Internal class for overiding compare in priority_queue
  class CompareOverRide {
  public:
    bool operator() (Node* left, Node* right) {
      return left->frequency > right->frequency;
    }
  };

  //private variables
  std::ifstream inFile;
  std::ofstream outFile;
  std::string inFileName, outFileName;
  std::string inFileText, outFileText;
  const int UIDSize = 4;
  const std::string uniqueID = "1234";
  std::string flag;
  int numBuffZeros;

  //encoded file segments
  std::string uniqueIDtoCheck; //4 byte "magic number"
  std::string encodedTree; //huff tree storage space, need byte length?
  std::string encodedMessage; //full encoded message
  std::string bufferZeros; //trailing zeros added, also acts as EOF indicator

  Node* root;
  std::vector<Node> arr; //may not be using?
  std::unordered_map<char, int> freqMap;
  std::unordered_map<char, std::string> huffMap;
  std::unordered_map<char, int> huffMapCompressed;
  int huffMapSize, trimSize;
  std::priority_queue<Node*, std::vector<Node*>, CompareOverRide> minHeap;

  //private functions
  Node* makeNewNode(char, int, Node*, Node*);
  void getInputText();
  void getUID();
  void getTreeSize();
  void getHuffTree();
  void getInputMessage();
  void getBufferZeros();
  void buildFreqMap(const std::string); //builds freq map for new text strings
  void buildPQ(); //builds PQ based off current freqMap
  void createTree();
  //void createHuffMap();
  void createEncodedString();
  void createEncodedTree();
  void createEncodedOutput();
  void createOutput();
  void buildTree(char, std::string&); //builds tree from char and path to that char ex(a, '101001')
  void traverse(Node*, std::string);

  int huffCodeToDec(std::string);
  std::string charDectoHuffCode(int);
  bool checkUID();


public:
  
  void doEncode();
  void doDecode();

  Huffman(std::string f, std::string ifName, std::string ofName) {
    this->inFileName = ifName;
    this->outFileName = ofName;
    this->flag = f;
  }

  //~Huffman(); //might not need for cleanup

};
#endif // !HUFFMAN_HPP
