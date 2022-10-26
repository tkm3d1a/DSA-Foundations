/*
* Author: Tim Klimpel
* Assignment Title: Big Multiplication
* Assignment Description: Multiply large values, up to 64 digits in length
* Due Date: 5/20/2022
* Date Created: 5/14/2022
* Date Last Modified: 5/18/2022
*/

/*
* Input:
*	Input can be either through the CLI when calling the program
*		- This expects a releative file path, and only expects a single file. Will instruct if too many arguments passed
*	Or input can be in the name of a file
*		- This expects a relative file path
*	If File path is incorrect in either case, user is notified, and default values are used.
*	If the user wants to input strings indivdually, they can choose to do so by inputting "no" at the file input request line
*	All input is stored in a vector for retrieval during algorithm execution
*	
* Process:
*	Takes input of integers and stores them in a string
*		- ALL inputs are stored as strings, there is no current "breakover" point for int->string (point of improvement)
*	Multiply the two inputs together utilizing Karatsuba's Algorithm and recursion
*		- Basic steps:
*			1. Equalize string lengths by prepending 0's to string till both lengths are the same
*			2. Base Case AFTER equalizing
*				a. checks for digit length of 1, then multiplies as int and returns the result as a string
*			3. Pick a midpoint, and use that to determine the "shift"
*				a. Shift is used to determine the power for the base later on
*			4. Midpoint is used to split string into two halves, 0->midpoint is first half, and midpoint+1->end of string is second half
*			5. directly solve for Z2 and Z0 (simple recurrsion call for multiplication)
*			6. Solve for z1 using the formula detailed in the wikipedia (allows for 1 less total recursion call)
*			7. Use midpoint shift to determine 0's required to append based off(because base is set at 10, appending 0's is trivial)
*				a. This was point of issue in dev.  Need to make sure to use the SHIFT value for determining the "m" value used here
*			8. Add z values together, and return the string
*	Two helper functions built to assist.  string addition function, and a function to make two input strings equal.
*		- MakeEqual modifies the string itself and returns the resulting length of the two strings based off the longest original string
*
* Output:
*	Outputs the result as a string value.  No truncation or modification, just the final multiplication is returned.
*	Output is returned to the console as well as to a new file titled "bigMultResults.txt"
*		- bigMultresults will also contain the input numbers to verify correctness of input if needed
* 
* Assumptions:
*	-All inputs are integers
*	-All file paths are relative
*/

/* includes go here */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const int ACII_SHIFT = 48; //I know, typo, also could handle with using '0' char as subtraction to correct for ASCII indexes
const int BASE = 10; //Not actually used, just showing that the BASE assumed is always value of 10

// Function declaration //

//Algorithm implementation of Karatsuba's multiplication
//Karatsuba's algorithm defintion from here: https://en.wikipedia.org/wiki/Karatsuba_algorithm
//Base case is when the passed strings are less than 2 digits 
std::string karaAlgo(std::string xString, std::string yString);

//addString has full functionality to add two strings like integers
//Strings can be any length, function corrects and returns correct addition
//Can handle subtraction by setting the boolean value to "true"
//Idea for this function came from: http://cplusplus.com/forum/general/91137/
std::string addString(std::string stringOne, std::string stringTwo, bool changeToMinus);

//Corrects strings to be equal length by pre-pending 0's to the shorter string
//Modifies the passed string directly and returns the length the strigns are set to
int makeEqualStrings(std::string &stringOne, std::string &stringTwo);

//Helper function to read file input into a vector for program usage
std::vector<std::string> readFileToVector(std::string fileName);

int main(int argc, char **argv) {

	std::string answer, userInput, fileName, outFileName = "bigMultResults.txt";
	std::vector<std::string> inputNumbers;
	bool useFile = false;

	switch (argc) {
	case 1:
		//user input of filename
		std::cout << "Input the name of the file containg numbers to multiply(put \'no\' to enable string input directly): ";
		std::cin >> fileName;
		if (fileName == "no") {
			break;
		}
		else {
			useFile = true;
			std::cout << std::endl << "Searching for file " << fileName << "..." << std::endl;
		}
		break;
	case 2:
		//filename is in CLI
		std::cout << "Retrieving file at: " << argv[1] << "..." << std::endl;
		fileName = argv[1];
		useFile = true;
		break;
	default:
		//unexpected number of arguments in cli call
		std::cout << "Too many arguments passed in file execution. Usage is: " << argv[0] << "<file relative path>" << std::endl;
		return 1;
		break;
	}
	
	if (useFile) {
		inputNumbers = readFileToVector(fileName);
	}
	else {
		std::cout << "Enter the first integer value: ";
		std::cin >> userInput;
		inputNumbers.push_back(userInput);
		std::cout << "Enter the second integer value: ";
		std::cin >> userInput;
		inputNumbers.push_back(userInput);
	}

	std::cout << "Now multiplying " << inputNumbers[0] << " by " << inputNumbers[1] << "..." << std::endl;
	answer = karaAlgo(inputNumbers[0], inputNumbers[1]);

	std::cout << std::endl << answer << std::endl;

	//Output file creation and writing
	std::ofstream outFile(outFileName); //outputfile handling, ofstream defaults ios::trunc in constructor
	std::cout << outFileName << " created.  Writing now..." << std::endl;
	for (auto i : inputNumbers) {
		outFile << "Input: " << i << std::endl;
	}
	outFile << "Result: " << answer << std::endl;
	outFile.close(); //output file closing
	std::cout << "Writing complete!" << std::endl;

	return 0;
}

// Function definition //

std::string karaAlgo(std::string xString, std::string yString)
{
	std::string stringOut = "debug1";
	int n = makeEqualStrings(xString, yString);

	//Base Case - If both input strings are only single digits
	if (n < 2) {
		int x = xString.at(0) - ACII_SHIFT;
		int y = yString.at(0) - ACII_SHIFT;
		int ans = x * y;
		stringOut = std::to_string(ans);
		/*std::cout << "Returning base case of: " << x << "*" << y << std::endl;*/
		return stringOut;
	}

	int midPoint = floor(n/2);
	int midPointShift = n - midPoint;

	//std::cout << "x: " << xString << " y: " << yString << " Mid: " << midPoint << " MidShift: " << midPointShift << " length: " << n << std::endl;

	std::string x_1 = xString.substr(0, midPoint);
	std::string x_0 = xString.substr(midPoint, midPointShift);
	std::string y_1 = yString.substr(0, midPoint);
	std::string y_0 = yString.substr(midPoint, midPointShift);
	std::string z_2, z_1, z_0, mSquared, m;

	//std::cout << "x_1: " << x_1 << " x_0: " << x_0 << std::endl;
	//std::cout << "y_1: " << y_1 << " y_0: " << y_0 << std::endl;

	z_2 = karaAlgo(x_1, y_1);
	/*std::cout << "z_2: " << z_2 << std::endl;*/
	z_0 = karaAlgo(x_0, y_0);
	/*std::cout << "z_0: " << z_0 << std::endl;*/
	z_1 = karaAlgo(addString(x_1, x_0, false), addString(y_1, y_0, false));
	/*std::cout << "z_1: " << z_1 << std::endl;*/
	z_1 = addString(z_1, z_2, true);
	/*std::cout << "z_1 - z_2: " << z_1 << std::endl;*/
	z_1 = addString(z_1, z_0, true);
	/*std::cout << "z_1 - z_0: " << z_1 << std::endl;*/


	for (int i = 0; i < midPointShift; i++) {
		m += "0";
		mSquared += "00";
	}

	z_2 += mSquared;
	z_1 += m;

	/*std::cout << "z_2: " << z_2 << " z_1: " << z_1 << " z_0: " << z_0 << std::endl;*/

	stringOut = addString(z_2, z_1, false);
	stringOut = addString(stringOut, z_0, false);
	/*std::cout << "Addition: " << stringOut << std::endl;*/

	return stringOut;
}

std::string addString(std::string stringOne, std::string stringTwo, bool changeToMinus)
{
	int n = makeEqualStrings(stringOne, stringTwo);

	std::string resStringRev = "", resString = "";
	int buffInt, remainder = 0;

	for (int i = n - 1; i >= 0; i--) {

		//The if statments can probably be wrapped up with something else instead of nesting this way
		if (changeToMinus && stringTwo.at(0) == '0') {
			buffInt = (int(stringOne.at(i))-ACII_SHIFT) - (int(stringTwo.at(i))-ACII_SHIFT) + remainder;
		}
		else if (changeToMinus && stringOne.at(0) == '0') {
			buffInt = (int(stringTwo.at(i)) - ACII_SHIFT) - (int(stringOne.at(i)) - ACII_SHIFT) + remainder;
		}
		else if (changeToMinus && stringOne.at(0) >= stringTwo.at(0)) {
			buffInt = (int(stringOne.at(i)) - ACII_SHIFT) - (int(stringTwo.at(i)) - ACII_SHIFT) + remainder;
		}
		else if (changeToMinus && stringOne.at(0) < stringTwo.at(0)) {
			buffInt = (int(stringTwo.at(i)) - ACII_SHIFT) - (int(stringOne.at(i)) - ACII_SHIFT) + remainder;
		}
		else {
			buffInt = int(stringOne.at(i) + stringTwo.at(i)) - (2 * ACII_SHIFT) + remainder;
		}

		remainder = 0;

		if (buffInt > 9) {
			buffInt -= 10;
			remainder = 1;
		}

		if (buffInt < 0) {
			buffInt += 10;
			remainder = -1;
		}

		resStringRev += std::to_string(buffInt);

		if (i <= 0 && remainder > 0) {
			resStringRev += std::to_string(remainder);
		}
	}

	while(resStringRev.at(resStringRev.length() - 1) == '0' && resStringRev.length() > 1) {
		resStringRev.pop_back();
	}
	
	resString = std::string(resStringRev.rbegin(), resStringRev.rend());

	return resString;
}

int makeEqualStrings(std::string &stringOne, std::string &stringTwo)
{
	int lengthOne = stringOne.length();
	int lengthTwo = stringTwo.length();

	//Only modifies if one length is greater than the other
	if (lengthOne > lengthTwo) {
		int leadingZeros = lengthOne - lengthTwo;
		for (int i = 0; i < leadingZeros; i++) {
			stringTwo = "0" + stringTwo;
		}
		return lengthOne;
	}
	else if (lengthTwo > lengthOne) {
		int leadingZeros = lengthTwo - lengthOne;
		for (int i = 0; i < leadingZeros; i++) {
			stringOne = "0" + stringOne;
		}
	}

	return lengthTwo;
}

std::vector<std::string> readFileToVector(std::string fileName)
{
	std::vector<std::string> inputStrings;
	std::string inputLine;
	
	std::fstream fio;
	fio.open(fileName);
	if (!fio.is_open()) {
		std::cout << "Error opening file.  Default values to be used (99 and 99)" << std::endl;
		inputStrings.push_back("99");
		inputStrings.push_back("99");
	}
	else {
		while (fio >> inputLine) {
			inputStrings.push_back(inputLine);
		}
	}

	return inputStrings;
}
