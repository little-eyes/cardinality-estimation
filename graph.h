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

class Neighbor {

public:
	Neighbor(int node, double probability);
	~Neighbor();
	int getId();
	double getTransitionProbability();
	void setTransitionProbability(double probability);
	void append(Neighbor *next);
	Neighbor *next();

private:
	int __NodeId;
	double __Probability;
	Neighbor *__Next;
};

class GraphNode {
	
public:
	GraphNode(int node, int size);
	~GraphNode();
	void addNeighbor(int node);
	void setTransitionProbability(int node, double probability);
	Neighbor *getNeighbors();
	int getDegree();
	int getNodeId();
	bool hasNeighbor(int node);

private:
	int __NodeId;
	Neighbor *__Neighbors;
	Neighbor *__LastNeighbor;
	int *__NeighborBitMap;
	int __Degree;
};

class Graph {

public:
	Graph(int nodes, double density);
	~Graph();
	/*
	 * Graph class public APIs.
	 */
	int getDegree(int node);
	Neighbor *getNeighbors(int node);
	bool hasNeighbor(int node, int neighbor);
	double getTransitionProbability(int nodei, int nodej);
	double getTransitionProbability(int node);
	//void dumpGraph(char *uri);
	//void dumpProbability(char *uri);
	void dumpGraphDegreeStatistics(char *uri);

private:
	int __NumberOfNodes;
	GraphNode **__GraphMap;
	void randomEdge(double density);
	void calculateTransitionProbability();
};

}

#endif
