/*
* Author: Tim Klimpel
* Assignment Title: 3-Way Merge Sort
* Assignment Description: This program prints the text Hello World to the console.
* Due Date: 5/13/2022
* Date Created: 5/07/2022
* Date Last Modified: 5/13/2022
*/

/*
* Input:
*	Takes user input from console to identify the file to be sorted
* 
* Process:
*	Verifies that user input can be opened, if it cannot, exits program
*	Once file is opened, reads file line by line to gather integers
*		- Utilizes vectors and push_back function to build list, not needing to know file input size at runtime
*	Once file is read completley, closes file
*	Sends new vector with unsorted numbers to Splitting function
*	Splitting function, first splits the vector using recursive calls till a vector of size 1 is obtained
*		- Split starts "left", then goes "m,iddle" and "right"
*	recombines using 3 way merge function
*		- Due to being "3 way", tracking of which passed vector is emptied first is tracked with a local boolean variable
*	Splitting function eventually returns a sorted vector to the main function once complete
*	A file is created to store the now sorted vector
*		- ofstream by default opens with ios::trunc, so if the file exists, it will be overwritten here
*	The sorted vector is written to the file
*	The file is closed
*	The main function ends
* 
* Output:
*	"sorted.txt" is output as the sorted version of the input file
*		- file is output to same directory as executable
*	if inputfile is not openable, function exits with no output
* 
* Assumptions:
*	user inputs correct filename they want sorted via console
*		- if file cannot be opened, this is handled, but if it is the incorrect file entirely, this is not
*		- File path is input as releative path
*	file to be sorted is structured as identified in problem statement
*		- int only
*		- one int per line
*/


#include <iostream>
#include <vector>
#include <fstream>
 
//function declarations

//Merging function
//This function handles the merging of the sorted arrays
std::vector<int> mergeThree(std::vector<int> vecLeft,
	std::vector<int> vecMiddle,
	std::vector<int> vecRight);

//Splitting function
//Main function, starts by splitting down to single element sorted arrays then utilizes merge function
//to recombine the arrays, sorting as it combines
std::vector<int> splitThree(std::vector<int> inputVec);

int main()
{
	//Init variables needed below
	std::vector<int> testInput;
	std::string fileName, outFileName = "sorted.txt";
	int a;

	//User input of file name to sort
	std::cout << "Input the name of the file to sort: ";
	std::cin >> fileName;
	std::cout << std::endl << "Searching for file " << fileName << "..." << std::endl;

	//File open and reading into testInput Vector
	std::fstream fio;
	fio.open(fileName);
	if (!fio) {
		std::cout << std::endl << "Error opening \'" << fileName << "\'. Please try again.";
		return 0;
	}
	else {
		std::cout << std::endl << "Successfully opening \'" << fileName << "\'. Now sorting..." << std::endl;
	}
	while (fio >> a) {
		testInput.push_back(a);
	}
	fio.close(); //input file closing

	//3 way merge sort call
	std::cout << "Sorting starting..." << std::endl;
	testInput = splitThree(testInput); //Can possibly improve by passing by reference? (not investigated)
	std::cout << "Sorting complete!" << std::endl;

	//Output file creation and writing
	std::ofstream outFile(outFileName); //outputfile handling, ofstream defaults ios::trunc in constructor
	std::cout << outFileName << " created.  Writing now..." << std::endl;
	for (auto i : testInput) {
		outFile << i << std::endl;
	}
	outFile.close(); //output file closing
	std::cout << "Writing complete!" << std::endl;

	return 0;
}

// MergeThree definition
std::vector<int> mergeThree(std::vector<int> vecLeft, 
	std::vector<int> vecMiddle, 
	std::vector<int> vecRight) {

	int i = 0, j = 0, k = 0, r = 0; //init STARTING counters
	int iStop = vecLeft.size();
	int jStop = vecMiddle.size();
	int kStop = vecRight.size();
	int rStop = iStop + jStop + kStop; //Fully merged vector should equal length of 3 passed vectors combined
	bool leftDone = false, midDone = false, rightDone = false; //cleaner calls

	//Create vector of size of all 3 inputs, fill with -999 for debugging
	std::vector<int> output(rStop, -999);

	//Step 1 of merging sorted arrays
	//Steps through each array, adding it to the output vector
	//Stops once it reahes the end of one of the input arrays
	while (i < iStop && j < jStop && k < kStop) {

		if (vecLeft[i] <= vecMiddle[j] && vecLeft[i] <= vecRight[k]) {
			output[r] = vecLeft[i];
			i++;
			r++;
		}
		else if (vecMiddle[j] <= vecLeft[i] && vecMiddle[j] <= vecRight[k]) {
			output[r] = vecMiddle[j];
			j++;
			r++;
		}
		else {
			output[r] = vecRight[k];
			k++;
			r++;
		}
		
		if (i == iStop) {
			leftDone = true;
		}
		if (j == jStop) {
			midDone = true;
		}
		if (k == kStop) {
			rightDone = true;
		}
	}

	//Now checking which array is done from previous while loop
	//(only 1 can be done at this point)
	if (leftDone) { //left vector done
		while (j < jStop && k < kStop) {
			if (vecMiddle[j] <= vecRight[k]) {
				output[r] = vecMiddle[j];
				j++;
				r++;
			}
			else {
				output[r] = vecRight[k];
				k++;
				r++;
			}
			if (j == jStop) {
				midDone = true;
			}
			if (k == kStop) {
				rightDone = true;
			}
		}
	}
	else if (midDone) { //middle vector done
		while (i < iStop && k < kStop) {
			if (vecLeft[i] <= vecRight[k]) {
				output[r] = vecLeft[i];
				i++;
				r++;
			}
			else {
				output[r] = vecRight[k];
				k++;
				r++;
			}
			if (i == iStop) {
				leftDone = true;
			}
			if (k == kStop) {
				rightDone = true;
			}
		}
	}
	else if (rightDone) { //right vector done
		while (i < iStop && j < jStop) {
			if (vecLeft[i] <= vecMiddle[j]) {
				output[r] = vecLeft[i];
				i++;
				r++;
			}
			else {
				output[r] = vecMiddle[j];
				j++;
				r++;
			}
			if (i == iStop) {
				leftDone = true;
			}
			if (j == jStop) {
				midDone = true;
			}
		}
	} //no final else to prevent possible missed boolean set

	//Now need to check on the double scenarios to identify last vector
	//	1.left and mid
	//	2.left and right
	//	3.mid and right
	if (leftDone && midDone) {
		while (k < kStop) {
			output[r] = vecRight[k];
			k++;
			r++;
		}
	}
	else if (leftDone && rightDone) {
		while (j < jStop) {
			output[r] = vecMiddle[j];
			j++;
			r++;
		}
	}
	else if (midDone && rightDone) {
		while (i < iStop) {
			output[r] = vecLeft[i];
			i++;
			r++;
		}
	}

	return output;
}

// splitThree definition
std::vector<int> splitThree(std::vector<int> inputVec) {
	//Setup base case at top
	if (inputVec.size() <= 1) {
		return inputVec;
	}

	//handling a vector size of exactly 2
	//second base case because of 3 way splitting
	if (inputVec.size() < 3) {
		if (inputVec[0] < inputVec[1]) { //2 item vector already sorted
			return inputVec;
		}
		else { //2 item vec not sorted, just swap index 0 and index 1 to sort
			int temp = inputVec[0];
			inputVec[0] = inputVec[1];
			inputVec[1] = temp;
			return inputVec;
		}
	}

	//getting sizes for all vectors
	//left and middle always equal, right split can be +1,0,-1 shifted in length
	int lSplit = inputVec.size() / 3;
	int mSplit = lSplit;
	int rSplit = inputVec.size() - lSplit - mSplit;

	std::vector<int> leftVec, midVec, rightVec;

	int r = 0;

	//splitting input into Left, middle, right and filling the vectors for each
	for (int i = 0; i < lSplit; i++) {
		leftVec.push_back(inputVec[r]);
		r++;
	}
	for (int i = 0; i < mSplit; i++) {
		midVec.push_back(inputVec[r]);
		r++;
	}
	for (int i = 0; i < rSplit; i++) {
		rightVec.push_back(inputVec[r]);
		r++;
	}

	//Recursive calls, always starting on left most values
	leftVec = splitThree(leftVec);
	midVec = splitThree(midVec);
	rightVec = splitThree(rightVec);

	//return directly using merge function
	return mergeThree(leftVec, midVec, rightVec);
}