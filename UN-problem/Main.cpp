/*
* Author: Tim Klimpel
* Assignment Title: The UN Problem
* Assignment Description: Given a set of attendees, determine the smallest amount of people
*	that need to be removed such that all remainng attendees can converse
* Due Date: 6/13/2022
* Date Created: 6/5/2022
* Date Last Modified: 6/13/2022
*/

/*
* Input:
*		- File to be provided in terminal when prompted
*		- File path to be relative to directory running program
* 
* Process:
*		-Convert file to 2 things, a number for error checking and determining final group size, and a vector of the remaining lines of the file
*			->This is done in approx O(n) time, where n is the number of lines in the file
*		-Split the above vector into component vectors of each indivdual string, but keep them under the same group for processing
*			->again, done in approximatley O(n) time, where n is the number of people (n-1 from above)
*		-Process the above vector<vector<string>> to produce 2 maps
*			-> this is done in O(n*m) time, where n is the number of unique names, and m is the maximum number of languages one name can understand (in our case this is 100*16 at worst case)
*			-> if language count approaches name count, it might be better to find another way to complete this process as it will approach O(n^2) in that case
*		-First map is <string, string> and has the Person and the language they speak
*		-Second map is <string, vector<string> and has the unique language name, and the vector of all the people that understand that language
*		-Then create a map <string, int> to assign a number to each unique name/language
*			->This is done in O(n+m), as the maximum loop time is max number of names + max number of languages
*			-with this process, all NAMES will be from 0 to toalNumPeople and the unique langauges will be the remainder
* Output:
* 
* Assumptions:
* 
*/

/*
* Resources:
*		Graph class: https://www.techiedelight.com/graph-implementation-using-stl/
*/

/*Includes*/
#include <iostream>
#include <fstream>
#include <stack>
#include <map>
#include <string>
#include <vector>
#include <iterator>

#include "Graph.h"

/*Namespaces*/
using namespace std;

/*Constants*/
const string FILENAME_TEST_1 = "twoNameTest.txt";
const string FILENAME_TEST_2 = "fiveNamesTest.txt";
const string FILENAME_TEST_3 = "sampleInput.txt";

/*Function Declaration*/

//Printing of graph for debuggin and final output
void printGraph(Graph const& graph, int n);

//Converting file of known format into vector and single integer
vector<string> readFileToStringVector(string fileName, int& nP);

//Splitting the single vector from readFileToStringVector into a vector of string vectors for map functions
vector<vector<string>> splitVectors(vector<string> input);

//Function for clearing the visted list
void clearVistList(vector<bool>& visited, int n);
void makeNameLangMaps(vector<vector<string>> input, map<string, string>& peopleToLang, map<string, vector<string>>& langToPeople);
void makeNodeMap(map<string, int>& nodeMap, map<string, string> peopleToLang, map<string, vector<string>> langToPeople);
vector<Edge> makeEdges(map<string, int> nodeMap, map<string, string> peopleToLang, map<string, vector<string>> langToPeople, int nP);
vector<Edge> reverseEdges(vector<Edge> edgeToRev);
void dfsBuildStack(const Graph& graph, stack<int>& stack, vector<bool>& isVisited, int visting);
void dfsVisit(const Graph& graph, vector<bool>& isVisited, int visiting, vector<int>& sccListComp);
void reverseTheNodeMap(const map<string, int>& nodeMap, map<int, string>& revNodeMap);
string getFileName();

//void kosajuru();


/******/
/*MAIN*/
/******/
int main() {
	
	map<string, string> peopleSpeak;
	map<string, vector<string>> langSpeak;
	map<string, int> nodeMap;
	map<int, string> revNodeMap;
	
	vector<vector<string>> splitInputVector;
	vector<vector<int>> sccList;
	vector<vector<string>> sccListReadable;

	vector<string> fileConversionVector;
	vector<Edge> edges;
	vector<Edge> revEdges;
	vector<bool> isVisited;

	int numPeople = -1;
	int nodeCount = -1;
	int largestGroup = -1;
	int largestGroupInd = -1;
	int toRemove = -1;

	string fileName = "";

	stack<int> stack;

	//fileConversionVector = readFileToStringVector(FILENAME_TEST_1, numPeople);
	//fileConversionVector = readFileToStringVector(FILENAME_TEST_2, numPeople);
	//fileConversionVector = readFileToStringVector(FILENAME_TEST_3, numPeople);

	fileName = getFileName();
	fileConversionVector = readFileToStringVector(fileName, numPeople);

	if (numPeople != -1 && fileConversionVector.size() == numPeople) {
		splitInputVector = splitVectors(fileConversionVector);
	}
	else {
		cout << "***ERROR***\nError in file input, please check filename "
			"and file contents before retrying\n***ERROR***" << endl;
		return 0;
	}

	makeNameLangMaps(splitInputVector, peopleSpeak, langSpeak);
	makeNodeMap(nodeMap, peopleSpeak, langSpeak);
	reverseTheNodeMap(nodeMap, revNodeMap);
	nodeCount = nodeMap.size();
	for (int i = 0; i < nodeCount; i++) {
		isVisited.push_back(false);
	}
	edges = makeEdges(nodeMap, peopleSpeak, langSpeak, numPeople);
	revEdges = reverseEdges(edges);

	Graph regularGraph(edges, nodeCount);
	Graph revGraph(revEdges, nodeCount);

	cout << "Regular Graph\n";
	printGraph(regularGraph, nodeCount);

	cout << "\nReversed Graph\n";
	printGraph(revGraph, nodeCount);


	/************************************************************************
	* Kosaraju's Algo implementation                                        *
	* This can likely be refactored into the graph class for easier reading *
	************************************************************************/
	for(int i = 0; i < nodeCount; i++){ //Need to loop through all nodes incase fully disconnected pieces
		if (!isVisited[i]) {
			dfsBuildStack(regularGraph, stack, isVisited, i);
		}
	}
	
	clearVistList(isVisited, nodeCount);
	
	while (!stack.empty()) {
		int stackTop = stack.top();
		stack.pop();
		if (!isVisited[stackTop]) {
			vector<int> sccComp;
			dfsVisit(revGraph, isVisited, stackTop, sccComp);
			sccList.push_back(sccComp);
		}
	}
	/************************************************************************
	* End of K's Implementation                                             *
	************************************************************************/

	for (int i = 0; i < sccList.size(); i++) {
		int count = 0;
		vector<int> tempInd;
		for (int j = 0; j < sccList[i].size(); j++) {
			if (sccList[i][j] < numPeople) {
				tempInd.push_back(sccList[i][j]);
			}
		}
		vector<string> temp;
		for (int j = 0; j < tempInd.size(); j++) {
			string name = revNodeMap[tempInd[j]];
			temp.push_back(name);
		}
		sccListReadable.push_back(temp);
	}



	cout << endl << "Groups that can converse: \n";
	for (int i = 0; i < sccListReadable.size(); i++) {
		cout << "   Group " << i+1 <<":\t";
		for (int j = 0; j < sccListReadable[i].size(); j++) {
			cout << sccListReadable[i][j] << " ";
		}
		cout << endl;
		int groupSize = sccListReadable[i].size();
		if (groupSize > largestGroup) {
			largestGroup = groupSize;
			largestGroupInd = i;
		}
	}

	cout << "\nThe largest group was group number " << largestGroupInd + 1 << " and they had " << largestGroup << " members that can converse!" << endl;
	toRemove = numPeople - largestGroup;

	cout << "From the original group of " << numPeople << ", the least amount of people you can remove is " << toRemove << ".\n";
	return 0;
}
/**********/
/*END MAIN*/
/**********/


/*Function Definitions*/
void printGraph(Graph const& graph, int n)
{
	for (int i = 0; i < n; i++) {
		cout << i << "---> ";
		for (int v : graph.adjList[i]) {
			cout << v << " ";
		}
		cout << endl;
	}
}

vector<string> readFileToStringVector(string fileName, int& nP)
{
	vector<string> inputStrings;
	string inputLine;

	fstream fio;
	fio.open(fileName);
	if (!fio.is_open()) {
		cout << "Error opening file." << endl;
		inputStrings.push_back("-1");
		inputStrings.push_back("ERROR");
	}
	else {
		while (getline(fio, inputLine)) {
			inputStrings.push_back(inputLine);
		}
	}

	fio.close();

	nP = stoi(inputStrings[0]);
	inputStrings.erase(inputStrings.begin());

	return inputStrings; //output: {"Jack English Spainish", "Bob Spainish English"}
}

vector<vector<string>> splitVectors(vector<string> input)
{
	size_t pos = 0;
	int count = 0;
	string spaceDelimiter = " ";
	vector<vector<string>> output;

	for (const auto& vecString : input) {
		vector<string> tempVec;
		string tempString = vecString;
		while ((pos = tempString.find(spaceDelimiter)) != string::npos) {
			tempVec.push_back(tempString.substr(0, pos));
			tempString.string::erase(0, pos + spaceDelimiter.length());
		}
		tempVec.push_back(tempString.substr(0, pos));
		output.push_back(tempVec);
	}

	return output;
}

void clearVistList(vector<bool>& visited, int n)
{
	for (int i = 0; i < n; i++) {
		visited[i] = false;
	}
}

void makeNameLangMaps(vector<vector<string>> input, map<string, string>& peopleToLang, map<string, vector<string>>& langToPeople)
{
	for (int i = 0; i < input.size(); i++) {
		peopleToLang[input[i][0]] = input[i][1];
		for (int j = 1; j < input[i].size(); j++) {
			langToPeople[input[i][j]].push_back(input[i][0]);
		}
	}

	//break
}

void makeNodeMap(map<string, int>& nodeMap, map<string, string> peopleToLang, map<string, vector<string>> langToPeople)
{
	int count = 0;

	for (const auto& item : peopleToLang) {
		nodeMap[item.first] = count;
		count++;
	}

	for (const auto& item : langToPeople) {
		nodeMap[item.first] = count;
		count++;
	}

}

vector<Edge> makeEdges(map<string, int> nodeMap, map<string, string> peopleToLang, map<string, vector<string>> langToPeople, int nP)
{
	vector<Edge> newEdges;
	int startEdge=-1, endEdge=-1;

	for (const auto& entry : nodeMap) {
		startEdge = entry.second;

		if (startEdge < nP) {
			string key = peopleToLang[entry.first];
			endEdge = nodeMap[key];
			newEdges.push_back({ startEdge, endEdge });
		}
		else {
			vector<string> temp = langToPeople[entry.first];
			for (int i = 0; i < temp.size(); i++) {
				endEdge = nodeMap[temp[i]];
				newEdges.push_back({ startEdge, endEdge });
			}
		}
		
	}

	return newEdges;
}

vector<Edge> reverseEdges(vector<Edge> edgeToRev)
{
	vector<Edge> reversedEdges;
	for (int i = 0; i < edgeToRev.size(); i++) {
		int tempStart = edgeToRev[i].destination;
		int tempEnd = edgeToRev[i].source;
		reversedEdges.push_back({ tempStart, tempEnd });
	}

	return reversedEdges;
}

void dfsBuildStack(const Graph& graph, stack<int>& stack, vector<bool>& isVisited, int visiting)
{
	isVisited[visiting] = true;
	vector<int> node = graph.adjList[visiting];
	int nodeNeighborsSize = node.size();

	for (int i = 0; i < nodeNeighborsSize; i++) {
		if (!isVisited[node[i]]) {
			dfsBuildStack(graph, stack, isVisited, node[i]);
		}
	}

	stack.push(visiting);
}

void dfsVisit(const Graph& graph, vector<bool>& isVisited, int visiting, vector<int>& sccListComp)
{
	isVisited[visiting] = true;
	vector<int> node = graph.adjList[visiting];
	int nodeNeighborsSize = node.size();

	for (int i = 0; i < nodeNeighborsSize; i++) {
		if (!isVisited[node[i]]) {
			dfsVisit(graph, isVisited, node[i], sccListComp);
		}
	}

	sccListComp.push_back(visiting);
}

void reverseTheNodeMap(const map<string, int>& nodeMap, map<int, string>& revNodeMap)
{
	for (const auto& item : nodeMap) {
		revNodeMap[item.second] = item.first;
	}
}

string getFileName()
{
	string fileName;
	char yesNo;
	cout << "Please enter the name of the file to be read:\n\t";
	cin >> fileName;
	cout << "\nWill utilize the following file\n.\\" << fileName << endl;
	cout << "Does that look correct(y/n)? ";
	cin >> yesNo;
	return fileName;
}
