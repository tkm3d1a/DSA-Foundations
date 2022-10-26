/*
* Author: Tim Klimpel
* Assignment Title: The UN Problem
* Assignment Description: Given a set of attendees, determine the smallest amount of people
*	that need to be removed such that all remainng attendees can converse
* Due Date: 6/13/2022
* Date Created: 6/5/2022
* Date Last Modified: 6/9/2022
*/

#pragma once
#include <iostream>
#include <vector>

// Struct for edge
struct Edge {
  int source, destination;
};

class Graph
{
private:

public:
  std::vector<std::vector<int>> adjList;

  Graph(std::vector<Edge> const& edges, int n);
  
};

