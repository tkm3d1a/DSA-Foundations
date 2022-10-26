#include "Huffman.hpp"

void Huffman::doEncode()
{
  buildPQ();
  createTree();
  createEncodedString();
  createEncodedTree();
  createEncodedOutput();
}

void Huffman::doDecode()
{
  //std::cout<<"\nEnter...";
  getInputText(); //std::cout<<"Exit getInputText()...\n";
  getUID(); //std::cout<<"Exit getUID()...\n";
  if (!checkUID()) {
    //std::cout << "\nUID of file does not match.  exiting program.\n";
    return;
  }
  getTreeSize(); //std::cout<<"Exit getTreeSize()...\n";
  getHuffTree(); //std::cout<<"Exit getHuffTree()...\n";
  getBufferZeros(); //std::cout<<"Exit getBufferZeros()...\n";
  getInputMessage(); //std::cout<<"Exit getInputMessage()...\n";
  createOutput(); //std::cout<<"Exit createOutput()...\n";
}

/* Node creation */
Node* Huffman::makeNewNode(char inputChar, int inputFreq, Node* left=nullptr, Node* right=nullptr)
{
  Node* newNodePtr = new Node(inputChar, inputFreq);
  newNodePtr->left = left;
  newNodePtr->right = right;
  return newNodePtr;
}

/*utility functions*/
int Huffman::huffCodeToDec(std::string subString)
{
  int count = subString.length() - 1;
  int val = 0;

  for (int i = 0; i < subString.length(); i++) {
    if (subString[i] == '1') {
      val += (int) pow(2, count); //need to cast pow() result as int
    }
    count--;
  }

  return val;
}

std::string Huffman::charDectoHuffCode(int numberFromChar)//Make sure unsigned char is cast
{
  std::string temp = "", result = "";

  while (numberFromChar > 0) {
    temp += (numberFromChar % 2 + '0');
    numberFromChar /= 2;
  }

  int padding = 8 - temp.length();
  temp.append(padding, '0');

  for (int i = temp.length() - 1; i >= 0; i--) {
    result += temp[i];
  }

  return result;
}

void Huffman::getInputText()
{
  unsigned char tempChar;
  if (this->flag == "-unhuff") {
    std::cout<<"\nReading file...\n";
    inFile.open(this->inFileName, std::ios::in | std::ios::binary);

    inFile.seekg (0, inFile.end);
    int length = inFile.tellg();
    inFile.seekg (0, inFile.beg);
    this->inFileText.reserve(length);
    
    inFile.seekg(0);

    unsigned char nextChar;
    inFile.read(reinterpret_cast<char*>(&nextChar), 1);
    while (!inFile.eof()) {
      this->inFileText += nextChar;
      inFile.read(reinterpret_cast<char*>(&nextChar), 1);
    }
    
  }
  else {
    std::cout<<"\nReading file...\n";
    inFile.open(this->inFileName, std::ios::binary);

    inFile.seekg (0, inFile.end);
    int length = inFile.tellg();
    inFile.seekg (0, inFile.beg);

    this->inFileText.reserve(length);
    this->outFileText.reserve(length);

    inFile.seekg(0);

    tempChar = inFile.get();
    while (!inFile.eof()) {
      this->inFileText += tempChar;
      tempChar = inFile.get();
    }
  }

  inFile.close();
  std::cout<<"File read\n";

}

void Huffman::buildTree(char charData, std::string& code)
{
  Node* current = root;
  for (int i = 0; i < code.length(); i++) {
    if (code[i] == '0') {
      if (current->left == nullptr) {
        current->left = makeNewNode('\0', 0);
      }
      current = current->left;
    }
    else if (code[i] == '1') {
      if (current->right == nullptr) {
        current->right = makeNewNode('\0', 0);
      }
      current = current->right;
    }
  }
  current->data = charData;
}


/*huff encoding flow*/
void Huffman::buildFreqMap(std::string text)
{
  for (char character: text) {
    this->freqMap[character]++;
  }
}

void Huffman::buildPQ()
{
  getInputText();
  buildFreqMap(this->inFileText);

  for (auto pair: this->freqMap) {
    this->minHeap.push(makeNewNode(pair.first, pair.second));
  }
}

void Huffman::createTree()
{
  std::priority_queue<Node*, std::vector<Node*>, CompareOverRide> tempPQ(minHeap);

  while (tempPQ.size() > 1) {
    Node* left = tempPQ.top(); tempPQ.pop();
    Node* right = tempPQ.top(); tempPQ.pop();

    int freqSum = left->frequency + right->frequency;
    tempPQ.push(makeNewNode('\0', freqSum, left, right));
  }

  this->root = tempPQ.top();
  traverse(this->root, "");
}

void Huffman::traverse(Node* entry, std::string code)
{
  if (entry->left == nullptr && entry->right == nullptr) {
    entry->pathInBin = code;
    this->huffMap[entry->data] = code; //creating huffmap at same time as building nodes
    return;
  }

  traverse(entry->left, code + '0');
  traverse(entry->right, code + '1');
}

void Huffman::createEncodedString()
{
  std::cout<<"\nEncoded string start\n";
  std::string encodedMsgLong;
  std::string tempMsg;

  for (char character : inFileText) {
    encodedMsgLong += huffMap[character];
    //substr must be called as early as possible here
    //time complexity depends on length of substring so if left outside it 
    // becomes really slow on large strings.
    while (encodedMsgLong.length() > 8) { 
      tempMsg += (char)huffCodeToDec(encodedMsgLong.substr(0, 8));
      encodedMsgLong = encodedMsgLong.substr(8);
    }
  }



  int zeroPadCount = 8 - encodedMsgLong.length();
  if (encodedMsgLong.length() < 8) {
    encodedMsgLong.append(zeroPadCount, '0');
  }

  tempMsg += (char)huffCodeToDec(encodedMsgLong);

  
  this->bufferZeros += (char)zeroPadCount; //bufferzeros saving
  this->encodedMessage = tempMsg; //encoded message saving
  std::cout<<"\nEncoded string end\n";
}

void Huffman::createEncodedTree()
{
  //std::cout<<"\nEncoded Tree start\n";
  std::string temp = "";
  std::string interiorTemp;

  //unsure if string reserves needed.  Will leave in for now.
  //Real issue seems to be the code bit length, need to find possible work around?
  temp.reserve(this->inFileText.length());
  interiorTemp.reserve(this->inFileText.length());
  this->encodedTree.reserve(this->inFileText.length());

  char treeSize = (char) this->huffMap.size();
  this->encodedTree += treeSize;
  
  //std::cout<<"\npoint 1\n";

  for (auto pair : huffMap) {
    char data = pair.first;
    temp += data;

    //std::cout<<"Pair.first: "<<pair.first<<" -- Pair.second: "<<pair.second<<"\n";

    std::string code = pair.second;
    int padding = 15 - static_cast<int>(code.length()); //String errors occur here if code length exceeds 15 bits.
    //Tried bumping to allow for 23 bit code lengths but could not figure out seg faults that were occuring.
    interiorTemp.append(padding, '0');
    interiorTemp += '1';
    interiorTemp += code;

    temp += (char)huffCodeToDec(interiorTemp.substr(0, 8));
    interiorTemp = interiorTemp.substr(8);
    temp += (char)huffCodeToDec(interiorTemp);
    interiorTemp.clear();
  }
  //std::cout<<"\npoint 2\n";
  this->encodedTree += temp;
  //std::cout<<"\nEncoded Tree end\n";

}

void Huffman::createEncodedOutput()
{
  outFile.open(outFileName, std::ios::out | std::ios::binary);

  outFile.write(this->uniqueID.c_str(), this->uniqueID.size());
  outFile.write(this->encodedTree.c_str(), this->encodedTree.size());
  outFile.write(this->encodedMessage.c_str(), this->encodedMessage.size());
  outFile.write(this->bufferZeros.c_str(), this->bufferZeros.size());

  outFile.close();
}

/*huff decoding flow*/
void Huffman::getUID()
{
  for (int i = 0; i < this->UIDSize; i++) {
    this->uniqueIDtoCheck += inFileText[i];
  }
}

bool Huffman::checkUID()
{
  return uniqueIDtoCheck == uniqueID;
}

void Huffman::getTreeSize()
{
  int sizeLoc = UIDSize;
  this->huffMapSize = (unsigned char) inFileText[sizeLoc];
}

void Huffman::getHuffTree()
{
  int bytesToRead = this->huffMapSize * 3;
  this->trimSize = this->UIDSize + 1 + bytesToRead; //trimsize should equal 4 + 1 + mapsize*3
  char tempChar;
  std::string tempString;
  this->root = makeNewNode('\0', 0);

  for (int i = this->UIDSize+1; i < bytesToRead+5; ) {
    tempChar = this->inFileText[i]; i++;

    tempString += charDectoHuffCode((unsigned char) this->inFileText[i]); i++;
    tempString += charDectoHuffCode((unsigned char) this->inFileText[i]); i++;
    //tempString += charDectoHuffCode((unsigned char) this->inFileText[i]); i++;
    //temp string has 3 bytes for path of char
    //temp string should be 3 bytes "00000000 00000000 00101011" like this
    //leading zeros and first '1' are garbage, then read path after
    int j = 0;
    while (tempString[j] == '0') { //count padding 0's
      j++;
    }
    tempString = tempString.substr(j + 1); //remove padding 0's + single '1'

    buildTree(tempChar, tempString);
    tempString.clear();
  }

  //removing front of file here to make tracking counting easier in later steps
  this->inFileText = this->inFileText.substr(this->trimSize);
}

void Huffman::getBufferZeros()
{
  this->numBuffZeros = this->inFileText.back();
  this->inFileText.pop_back();
}

void Huffman::getInputMessage() //after changing to huff code being 4 bytes, have seg fault here...
{
  std::string tempPath;
  Node* current = root;
  //std::cout<<"Outside for loop...\n";
  for (int i = 0; i < this->inFileText.size(); i++) {
    tempPath = charDectoHuffCode((unsigned char)this->inFileText[i]);
    //std::cout<<"tempPath: "<<tempPath<<"\n";

    if (i == this->inFileText.size() - 1) {
      tempPath = tempPath.substr(0, 8 - this->numBuffZeros);
    }

    for (int j = 0; j < tempPath.size(); j++) {
      if (tempPath[j] == '0') {
        current = current->left;
        //std::cout<<"Went left... " << current << "\n";
      }
      else {
        current = current->right;
        //std::cout<<"Went right... " << current << "\n";
      }

      if (current->left == nullptr && current->right == nullptr) {
        //std::cout<<"At leaf\n";
        this->outFileText += current->data;
        current = root;
      }
    }
  }
}

void Huffman::createOutput()
{
  outFile.open(outFileName, std::ios::out | std::ios::binary);
  outFile.write(this->outFileText.c_str(), this->outFileText.size());
  outFile.close();
}