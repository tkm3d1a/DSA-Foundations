/*
* Author: Tim Klimpel
* Assignment Title: Huffman Coding
* Assignment Description: Implementing Huffman encoding.  Both Encoding and Decoding are to be
  completed and called using the -huff and -unhuff flag at runtime
* Due Date: 2022/08/01
* Date Created: 2022/07/20
* Date Last Modified: 2022/08/01
*/

/************************/
/*        NOTES         */
/************************/
// I ran out of time to complete full code documentation.
// Code works as described in problem statment from canvas.
// One single error found in testing: When trying to compress
//    a large file (bible.txt, ~4MB), I consistently get string errors
//  My attempt to fix them led to more errors, can see comments in Huffman.cpp
//    to see where attempts were going. All debug statments left in as well
// Not my cleanest code, but this will become a bigger project with GUI eventually.
// Cleanup and documentation to be added then.

/*
* Input:
* Process:
* Output:
* Assumptions:
  - No bit codes will exceed 15 bits in length
*/

/*Resources*/
/*includes*/
#include <iostream>
#include "Huffman.hpp"

/*Namespaces*/
//all namespace control in header file

/*Constants*/
/*Struct*/
/*Classes*/
// Huffman(string flag, string infile, string outfile)

/*Function Declerations*/

/*************
* MAIN START *
*************/

int main(int argc, char *argv[])
{
  if (argc != 4) {
    std::cout << "Not enough arguments.  Please run the executable with the following arguments:\n\n" << 
      "<flag> <source> <destination>\n\n" <<
      "Exiting program...\n";
    return 1;
  }

  std::string flag = argv[1];
  std::string inFile = argv[2];
  std::string outFile = argv[3];
  Huffman fileForWork(flag, inFile, outFile);

  if (flag == "-huff") {
    fileForWork.doEncode();
  }
  else if (flag == "-unhuff") {
    fileForWork.doDecode();
  }
  else {
    std::cout << "**ERROR**\n" << 
      "Invalid flag input -> " << flag << "\n" << 
      "Acceptable flag options: \'-huff\' or \'-unhuff\'\n\n";
  }

  return 0;
}

/*************
* MAIN END   *
*************/

/*Function definitions*/