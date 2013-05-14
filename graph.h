/*
 * The Approxmiated Cardinality Estimation Project
 * Author: Jilong Liao (jliao2@utk.edu)
 * 
 * Graph class and interfaces.
 **/

#ifndef _GRAPH_H
#define _GRAPH_H

#include <vector>
#include <algorithm>


namespace std {

class Graph {

public:
	Graph(int n, double density);
	int getDegree(int node);
	vector < pair<double, int> > getNeighbors(int node);
	double getTransitionProbability(int nodei, int nodej);
	double getTransitionProbability(int nodei);
	void printGraph();

private:
	int __NumberOfNodes;
	vector < vector<int> > __GraphMap;
	vector < vector<double> > __TransitionProbability;
	void randomEdge(double density);
	void calculateTransitionProbability();
};

}

#endif
