/*
* Author: Tim Klimpel
* Assignment Title: Galactic Bonding
* Assignment Description: Given a set of test cases and star locations, identify the number of
*   constelations in each test case.  Output the results using standard output(cout)
* Due Date: 7/11/2022
* Date Created: 7/8/2022
* Date Last Modified: 7/10/2022
*/

/*
* Input:
* Standard cin is used as directed in the input section of the homework assignment page
* Expectation of input is as follows:
* - 1 line with total number of test cases
* - 1 line to start each test case, with the number of stars in that case(int) and the
*     max distance (double) between stars for them to be grouped
* - n lines of double pairs to identify star coordinates for each test case
* -- n is determined in the first line of each test case
* 
* Process:
* Data is collected and ultimatley stored in the same index across three seperate vectors. From there,
* the individual nodes are stepped through and compared to all other nodes to gather edge distances.  Any edge distance
* that is under the maximum distance for that test case is then combined with the unionSet function.  This will build
* a tree for each "constellation" such that the 'root' will be the last node being processed. After this is completed, 
* the total sets are counted, and reported to the user.
* 
* Output:
* Each test case and that test case's total number of 'constellations'
*
* Assumptions:
* All values are input correctly and in the given order from the problem statement
* all values are delimited by a " " character.
*/

/*Resources*/
// Disjointed set class info pulled directly from canvas, no modifications at this time
// No other outside resources utilized
 
/*includes*/
#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>

/*Namespaces*/
using namespace std;

/*Constants*/
int MAX_STARS = 1000;

/*Struct*/
struct Edge {
  int source, destination;
  double cost;
};

/*Classes*/
//This is the class provided in the homework assignment.  No modifications made or required
// There are points of improvement possibly, but due to being required to build the dense graph, building
// already takes |v|^2 time.
class DisjointSet {
private:
  vector<int> pset;

public:

  DisjointSet(int MAX_STARS) {
    pset.assign(MAX_STARS, 0);
  }

  void initSet(int N) { // Takes N time
    for (size_t i = 0; i < N; i++) {
      pset[i] = i;
    }
  }

  int findSet(int i) { // Takes logn time (depth of tree is max logn, this is tree searching and returning the 'root', one way traversal)
    if (pset[i] == i) {
      return i;
    }
    else {
      return (pset[i] = findSet(pset[i]));
    }
  }

  bool isSameSet(int i, int j) { // Takes 2*logN (via find set function)
    return findSet(i) == findSet(j);
  }

  void unionSet(int i, int j) { // Takes logN time (via findSet function)
    pset[findSet(i)] = findSet(j);
  }

  int countSets(int N) {// Takes N time
    int count = 0;

    for (size_t i = 0; i < N; i++) {
      if (pset[i] == i) {
        count++;
      }
    }

    return count;
  }
};

/*Function Declerations*/

/*
* Description: calcDistance is for calculating distance using 2d points
* return: the distance as a double
* precondition: none
* postcondition: none
*/
double calcDistance(double x1, double x2, double y1, double y2);

/*************
* MAIN START *
*************/
int main()
{
  int testCase;
  vector<int> numStars;
  vector<double> maxDist;
  vector<vector<pair<double, double>>> coordinates;
  vector<DisjointSet> unions;

  //vector<Edge> testingEdge;
  //int edgeCount = 0;

  cout << "Enter values:\n";
  cin >> testCase;

  numStars.assign(testCase, 0);
  maxDist.assign(testCase, 0);
  unions.assign(testCase, DisjointSet(MAX_STARS));
  coordinates.assign(testCase, vector<pair<double,double>>(0));

  //Collection of data and parsing into same index of the three vectors
  //Note: can be refactored into a class, to make maintaining easier and to control redundancy
  //No refactoring completed at this time
  //TODO//
  for (int i = 0; i < testCase; i++) {
    cin >> numStars[i] >> maxDist[i];
    coordinates[i].assign(numStars[i], make_pair(0, 0));
    unions[i].initSet(numStars[i]);
    //cout << unions[i].countSets(numStars[i]);

    for (int j = 0; j < numStars[i]; j++) {
      cin >> coordinates[i][j].first >> coordinates[i][j].second;
      //Fills in coordinates of the i-th test case and the j-th star in that test case
    }
  }

  //Moving through each set of coordinates, calculating distance, and then completing 
  // the union if less than maxDist. This same process would be needed if building a full graph, just the graphing portion is removed
  //
  //Do n loops, where n is number of starts, 
  // First loop takes n, second n-1, third n-2... resulting in n^2 time
  //
  //There should be a faster way, but the limitation is making the graph itself, not the unionfinding
  for (int i = 0; i < coordinates.size(); i++) { //i here represents the test case number (first loop)
    for (int j = 0; j < coordinates[i].size(); j++) {//j represents the base star we are comparing from
      for (int k = j + 1; k < coordinates[i].size(); k++) {//k is used to compare the base star (j) to the remaining stars in the input
        //distance coord[i][j] -> [i][k]
        double distance = calcDistance(coordinates[i][j].first,
                                        coordinates[i][k].first,
                                        coordinates[i][j].second,
                                        coordinates[i][k].second);

        if(distance <= maxDist[i]) {

          //make an edge for testcase i
          //Only adds an edge if it is under the minimum distance
          //Adding the edge is not required anymore, but was used for testing
          //Can just directly union j and k if edge is under maxDist (tested as shown here)
          // testingEdge.resize(edgeCount + 1);
          // testingEdge[edgeCount].cost = distance;
          // testingEdge[edgeCount].source = j;
          // testingEdge[edgeCount].destination = k;
          unions[i].unionSet(j, k);
          // edgeCount++;
        }
      }
    }
    cout << endl << "Case " << i+1 << ": " << unions[i].countSets(numStars[i]);
  }

  return 0;
}
/*************
* MAIN END   *
*************/

/*Function definitions*/
double calcDistance(double x1, double x2, double y1, double y2){
  double dX = x2 - x1;
  double dY = y2 - y1;

  return sqrt((dX*dX)+(dY*dY));
}