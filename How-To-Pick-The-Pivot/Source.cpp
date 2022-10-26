/*
* Author: Tim Klimpel
* Assignment Title: How to pick a pivot
* Assignment Description: Using 4 different pivot selection techniques and analyzing the comparisions made on 4 different unsorted
*	files.  These results should then be presented in a readable and nice presentable format.
* Due Date: 5/30/2022
* Date Created: 5/20/2022
* Date Last Modified: 5/30/2022
* 
* Additional note: Testing git repo with new .gitignore template.  Need to verify it is
*	set up correctly.
*/

/*
* Input:
*	- Input is expected to be as described in the notes below.
*		- 4 files in total, all pulled from canvas assignment page.
* 
* Process:
*	- Take input files, and read them into vectors stored in an array for ease of access
*		- Statically set to expecting 5 files (1 test file and 4 provided files)
*		- If file is unable to be opened or read, a default set of values is provided and console message is logged
*	- The quicksort occurs on each file, using each of the 4 types of pivot selection
*		- quicksort is modified with a "mode" selection to enable this
*		- comparisons made are counted at the start of each call, using the subarray size - 1 as described in the assignment notes
*	- Each sorted vector is then tested to make sure all items are in order.  Results displayed on console
*	- Comparisons for each type are written to a file for ingestion into graphing programs
*		-CSV file format is used for this
* 
* Output:
*	- Console output of each vector's status for sorting, as well as the comparisons made
*	- File written contatining comparisons made for graph analysis
*	- CSV file for easy use in most excel-like programs
* 
* Assumptions:
*	- The CSV file will need to be manipulated in another program for "pretty" graph presentation
*	- Inputfilenames and locations match to exactly as listed below
		- This can be modified for CLI input at runtime, but is not done here
*	- If using an IDE, reserved stack size might need to be adjust to prevent stack overflow errors
*		-This was written in VS2022, /STACK:reserve was increased to 8MB to run this program without stackoverflow errors
* 
*/

//Notes
//Unsorted file locations: 
//		\UnSortedFiles\QuickSort10k.txt
//		\UnSortedFiles\QuickSort10kR.txt
//		\UnSortedFiles\QuickSort10kSemi2k.txt
//		\UnSortedFiles\QuickSort10kSemi4k.txt
//		\UnsortedFiles\ShortTest50.txt
//comparisons file locations: 
//		\SortedFiles

/* Include files */
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <ctime>

/* Constants */
const std::string OUTFILE_PREPEND = ".\\SortedFiles\\";
const std::string OUTFILE_APPEND = "OutputText.txt";
const std::string OUTFILE_CSV_APPEND = "OutputCSV.csv";

/* Function Declerations */

std::vector<int> readFileToVector(std::string fileName);

void writeVectorToFile(std::vector<int> outVector, std::string outFileName);

void writeCountertoFile(int counter, int mode, std::string outFileName);

/*
* partition Picks the pivot and partions the array around it using the left and right pointer to determine the subarray it is currently working on
* 
* NOTE: partitionQS and partitionYT are abandoned and no longer used in this file.  Data left to show where code was at one point.

int partitionQS(std::vector<int>& passedArray, int startPointer, int endPointer); //DO NOT USE
// based off partition seen here: https://www.youtube.com/watch?v=COk73cpQbFQ&t=1021s&ab_channel=mycodeschool
int partitionYT(std::vector<int>& arr, int start, int end); //DO NOT USE


*/

// Left uses the left most index of the passed subarray to pick the pivot.  Method for partitioning is as described in Online lecture notes
// All other partition functions are modified to be based off placing the pivot at the right most element of the passed subarray.
// This means the method for partioning is slightly different for the remaining three types, but they are consistent to not alter results/
int partitionLectureLeft(std::vector<int>& arr, int left, int right);

int partitionLectureRight(std::vector<int>& arr, int left, int right);

int partitionLectureRandom(std::vector<int>& arr, int left, int right);

int partitionLectureMedian(std::vector<int>& arr, int left, int right);

/*
* quickSortTK implments quick sort algorithim
* 
* counter - a reference variable to keep track of comparisons made through the sort.  Passed as reference to avoid global variables
* mode - a variable used to set the mode of partion being used.  Useful for running all 4 types in one execution.
*/
void quickSortTK(std::vector<int> &passedArray, int startPointer, int endPointer, int &counter, int mode);

/*
median of three function - helper function for return the median value of three passed integers
*/
int medianThree(int a, int b, int c);

/*
Helper function to test wether a passed vector is sorted or not.  Return value not currently used.
*/
bool testSort(std::vector<int> arr, int counter, int mode, std::string outFileName);

/*
Helper function to turn mode number back into what pivot selection was used for output into the CSV
*/
std::string getSortMode(int mode);

/**************/
/* Main Start */
/**************/

int main(int argc, char **argv) {

	srand(time(0)); //Setting up random time seed for use later

	//File name strings
	std::string inputFile1, inputFile2, inputFile3, inputFile4, inputFileNameTest, debugFileName = "debugFail";
	std::string outFileName, outFileCSV;
	int counter = 0;

	//inputFileNameTest = ".\\UnSortedFiles\\ShortTest50.txt";
	inputFile1 = ".\\UnSortedFiles\\QuickSort10k.txt";
	inputFile2 = ".\\UnSortedFiles\\QuickSort10kR.txt";
	inputFile3 = ".\\UnSortedFiles\\QuickSort10kSemi2k.txt";
	inputFile4 = ".\\UnSortedFiles\\QuickSort10kSemi4k.txt";
	outFileName = OUTFILE_PREPEND + OUTFILE_APPEND;
	outFileCSV = OUTFILE_PREPEND + OUTFILE_CSV_APPEND;

	int subStrCutPoint = 16;

	/*TESTING OF SMALLER INPUT VECTORS*/
	//std::vector<int> testVec0 = readFileToVector(inputFileNameTest);
	//std::vector<int> vecArr[] = { testVec0,testVec0,testVec0,testVec0 };
	//for (int i = 1; i <= 4; i++) {
	//	quickSortTK(vecArr[i-1], 0, testVec0.size() - 1, counter, i);
	//	testSort(vecArr[i-1], counter, i, outFileName);
	//	counter = 0;
	//}
	//std::cout << std::endl;
	/*TESTING OF SMALLER INPUT VECTORS*/
	
	std::vector<int> testVec1 = readFileToVector(inputFile1);
	std::vector<int> vecArr1[] = { testVec1,testVec1,testVec1,testVec1 };
	std::ofstream outFile(outFileName, std::ios_base::app);
	std::ofstream outCSV(outFileCSV);
	outCSV << "File Sorted,Sort Mode,Comparisons Made\n";
	outFile << "-------" << inputFile1.substr(subStrCutPoint) << "--------" << std::endl;
	for (int i = 1; i <= 4; i++) {
		quickSortTK(vecArr1[i - 1], 0, testVec1.size() - 1, counter, i);
		testSort(vecArr1[i - 1], counter, i, outFileName);
		outCSV << inputFile1.substr(subStrCutPoint) << "," << getSortMode(i) << "," << counter << "\n";
		counter = 0;
	}
	std::cout << std::endl;

	std::vector<int> testVec2 = readFileToVector(inputFile2);
	std::vector<int> vecArr2[] = { testVec2,testVec2,testVec2,testVec2 };
	outFile << "--------" << inputFile2.substr(subStrCutPoint) << "--------" << std::endl;
	for (int i = 1; i <= 4; i++) {
		quickSortTK(vecArr2[i - 1], 0, testVec2.size() - 1, counter, i);
		testSort(vecArr2[i - 1], counter, i, outFileName);
		outCSV << inputFile2.substr(subStrCutPoint) << "," << getSortMode(i) << "," << counter << "\n";
		counter = 0;
	}
	std::cout << std::endl;

	std::vector<int> testVec3 = readFileToVector(inputFile3);
	std::vector<int> vecArr3[] = { testVec3,testVec3,testVec3,testVec3 };
	outFile << "-----" << inputFile3.substr(subStrCutPoint) << "----" << std::endl;
	for (int i = 1; i <= 4; i++) {
		quickSortTK(vecArr3[i - 1], 0, testVec3.size() - 1, counter, i);
		testSort(vecArr3[i - 1], counter, i, outFileName);
		outCSV << inputFile3.substr(subStrCutPoint) << "," << getSortMode(i) << "," << counter << "\n";
		counter = 0;
	}
	std::cout << std::endl;

	std::vector<int> testVec4 = readFileToVector(inputFile4);
	std::vector<int> vecArr4[] = { testVec4,testVec4,testVec4,testVec4 };
	outFile << "-----" << inputFile4.substr(subStrCutPoint) << "----" << std::endl;
	for (int i = 1; i <= 4; i++) {
		quickSortTK(vecArr4[i - 1], 0, testVec4.size() - 1, counter, i);
		testSort(vecArr4[i - 1], counter, i, outFileName);
		outCSV << inputFile4.substr(subStrCutPoint) << "," << getSortMode(i) << "," << counter << "\n";
		counter = 0;
	}
	std::cout << std::endl;

	//File clean up incase it wasn't handled previously
	outFile.close();
	outCSV.close();
	return 0;
}

/**************/
/*  Main End  */
/**************/

/* Function definitions */

std::vector<int> readFileToVector(std::string fileName)
{
	std::vector<int> inputVector;
	int inputValue;

	std::fstream fio;
	fio.open(fileName);

	if (!fio.is_open()) {
		std::cout << "Error opening file \'" << fileName << "\'" << std::endl;
		for (int i = 0; i < 10; i+=2) {
			inputVector.push_back(i);
		}
	}
	else {
		while (fio >> inputValue) {
			inputVector.push_back(inputValue);
		}
	}

	return inputVector;
}

void writeVectorToFile(std::vector<int> outVector, std::string outFileName)
{
	std::ofstream outFile(outFileName); //Will overwrite if filenamealready exists, checking for this?
	std::cout << outFileName << " has been created.  Writing now." << std::endl;
	for (int i : outVector) {
		outFile << i << std::endl;
	}
	std::cout << "File writing complete." << std::endl;
}

void writeCountertoFile(int counter, int mode, std::string outFileName)
{
	std::string modeName;

	switch (mode) {
	case 1:
		modeName = "Left Pivot ------------- " + std::to_string(counter);
		break;
	case 2:
		modeName = "Right Pivot ------------ " + std::to_string(counter);
		break;
	case 3:
		modeName = "Random Pivot ----------- " + std::to_string(counter);
		break;
	case 4:
		modeName = "Median-of-three Pivot -- " + std::to_string(counter);
		break;
	default:
		std::cout << "Improper Mode passed, no file created." << std::endl;
		return;
	}

	std::cout << modeName << std::endl;
	std::ofstream outFile(outFileName, std::ios_base::app);
	outFile << modeName << std::endl;
	
}

/*
Partition seems to only work on the first time an array is passed.  It fails at sorting on the second pass through, but can't tell
where it is failing yet.  debugging hasn't been helpful...  Need to keep using print statements?

more update: no progress on fixing partioning... need to re-read book section and try to work it out there.  See concepts notes
for more details...

Confirmed with a YT partition algo, the book partition algo has some issue.  need to investigate before using

NOTE: Above info is old.  Was using wrong lecture notes to create partition from.  Have fixed and using the partition functions below QS and YT.
	Commented out code to remove from compiling, but left for review later or possible updates post-submission
*/
/*
int partitionQS(std::vector<int>& passedArray, int startPointer, int endPointer)
{
	int randRange = endPointer - startPointer;
	int randIndex = (rand() % randRange);

	int a = passedArray[startPointer];
	int midPoint = (endPointer - startPointer) / 2;
	int b = passedArray[midPoint];
	int c = passedArray[endPointer];
	int medianVal = medianThree(a, b, c);
	
	//last element
	//int pivot = passedArray[endPointer];
	//first element (causes stackoverflow)
	//int pivot = passedArray[startPointer];
	//random element
	//int pivot = passedArray[randIndex];
	//median of 3
	int pivot = medianVal;

	std::cout << pivot << std::endl;

	std::vector<int> bArr;
	
	for (int val : passedArray) {
		bArr.push_back(val);
	}

	int bLeftPointer = startPointer;
	int bRightPointer = endPointer;

	for (int i = startPointer; i < endPointer; i++) {
		if (passedArray[i] <= pivot) {
			bArr[bLeftPointer] = passedArray[i];
			bLeftPointer++;
		}

		if (passedArray[i] > pivot) {
			bArr[bRightPointer] = passedArray[i];
			bRightPointer--;
		}
	}

	bArr[bLeftPointer] = pivot;

	for (int i = 0; i < bArr.size(); i++) {
		passedArray[i] = bArr[i];
	}

	return bLeftPointer;
}
int partitionYT(std::vector<int>& arr, int start, int end)
{
	int pivot = arr[end];
	int pIndex = start;

	for (int i = start; i < end; i++) {
		if (arr[i] <= pivot) {
			std::swap(arr[i], arr[pIndex]);
			pIndex++;
		}
	}

	std::swap(arr[pIndex], arr[end]);
	return pIndex;
}
*/

int partitionLectureLeft(std::vector<int>& arr, int left, int right)
{
	int pivot = arr[left];
	int i = left+1;

	for (int j = left + 1; j <= right; j++) {
		if (arr[j] < pivot) {
			std::swap(arr[j], arr[i]);
			i++;
		}
	}

	i--;

	std::swap(arr[i], arr[left]);
	return i;
}

int partitionLectureRight(std::vector<int>& arr, int left, int right)
{
	int pivot = arr[right];
	int i = left;

	for (int j = left; j < right; j++) {
		if (arr[j] < pivot) {
			std::swap(arr[j], arr[i]);
			i++;
		}
	}

	//i--;

	std::swap(arr[i], arr[right]);
	return i;
}

int partitionLectureRandom(std::vector<int>& arr, int left, int right) // need to figure out second half swapping?  It seems to order everything fine up until the last steps.
{

	int randRange = right - left;
	int randIndex = (rand() % randRange) + left;
	int pivot = arr[randIndex]; //pick pivot
	std::swap(arr[randIndex], arr[right]); //swap pivot with right end point for swapping at end
	int i = left;

	for (int j = left; j < right; j++) {
		if (arr[j] < pivot) {
			std::swap(arr[j], arr[i]);
			i++;
		}
	}

	//i--;

	std::swap(arr[i], arr[right]);
	return i;
}

int partitionLectureMedian(std::vector<int>& arr, int left, int right)
{
	int middleIndex = (right - left) / 2;
	int midVal = arr[middleIndex];
	int leftVal = arr[left];
	int rightVal = arr[right];

	int pivot = medianThree(midVal, leftVal, rightVal);
	if (pivot == leftVal) {
		std::swap(arr[left], arr[right]);
	}
	else if(pivot == midVal) {
		std::swap(arr[middleIndex], arr[right]);
	} //implied if none of the above, the right val was selected and no swap required
	
	int i = left;

	for (int j = left; j < right; j++) {
		if (arr[j] < pivot) {
			std::swap(arr[j], arr[i]);
			i++;
		}
	}

	//i--;

	std::swap(arr[i], arr[right]);
	return i;
}

void quickSortTK(std::vector<int> &passedArray, int startPointer, int endPointer, int &counter, int mode)
{
	if (startPointer < endPointer) {
		counter += (endPointer - startPointer)-1; //subarray length - 1, added to the passed counter variable each time
		int pivot;
		switch (mode) {
		case 1:
			pivot = partitionLectureLeft(passedArray, startPointer, endPointer);
			break;
		case 2:
			pivot = partitionLectureRight(passedArray, startPointer, endPointer);
			break;
		case 3:
			pivot = partitionLectureRandom(passedArray, startPointer, endPointer);
			break;
		case 4:
			pivot = partitionLectureMedian(passedArray, startPointer, endPointer);
			break;
		default:
			std::cout << "Error with partition mode, exiting QS" << std::endl;
			return;
		}

		quickSortTK(passedArray, startPointer, pivot - 1, counter, mode);
		quickSortTK(passedArray, pivot + 1, endPointer, counter, mode);
	}
}

int medianThree(int a, int b, int c)
{
	if (((a > b) && !(a > c)) || ((a > c) && !(a > b))) { //supposed to be mimicing XOR operation 
		return a;
	}
	else if (((b>a) && !(b>c)) || ((b>c) && !(b>a))) {
		return b;
	}
	else {
		return c;
	}
}

bool testSort(std::vector<int> arr, int counter, int mode, std::string outFileName)
{
	int prevVal = arr[0];

	for (int i = 1; i < arr.size(); i++) {
		if (arr[i] < prevVal) {
			std::cout << "Incorrect sort at index: " << i << std::endl;
			return false;
		}
		else {
			prevVal = arr[i];
		}
	}

	//std::cout << "Cannot detect bad sort, consider it a win!" << std::endl;
	//std::cout << "Total comparisons made: " << counter << " | Mode run: " << mode << std::endl;
	writeCountertoFile(counter, mode, outFileName);
	return true;
}

std::string getSortMode(int mode)
{
	std::string sortModeString = "DEFAULT";

	switch (mode) {
	case 1:
		sortModeString = "First index pivot";
		break;
	case 2:
		sortModeString = "Last index pivot";
		break;
	case 3:
		sortModeString = "Random index pivot";
		break;
	case 4:
		sortModeString = "Median-of-three pivot";
		break;
	default:
		break;
	}
	return sortModeString;
}
