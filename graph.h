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

class GraphNode {
	
public:
	GraphNode(int node);
	void setTransitionProbability(double p);
	double getTransitionProbability();
	int getNodeId();

private:
	int __NodeId;
	double __Probability;
};

class Graph {

public:
	Graph(int n, double density);
	~Graph();
	/*
	 * Graph class public APIs.
	 */
	int getDegree(int node);
	vector < pair<double, int> > getNeighbors(int node);
	bool hasNeighbor(int node, int neighbor);
	double getTransitionProbability(int nodei, int nodej);
	double getTransitionProbability(int node);
	void printGraph();
	void dumpGraphStatistics();

private:
	int __NumberOfNodes;
	//vector < vector<GraphNode *> > __GraphMap;
	//vector <GraphNode *> __Nodes;
	int *__GraphMap;
	double *__ProbabilityMap;
	int *__DegreeTable;
	void randomEdge(double density);
	void calculateTransitionProbability();
	int PositionToIndex(int x, int y);
};

}

#endif
