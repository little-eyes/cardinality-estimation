/*
 * Approximated Cardinality Estimation Project
 * Author: Jilong Liao (jliao2@utk.edu)
 *
 **/

#include "graph.h"
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <algorithm>

namespace std {

GraphNode::GraphNode(int node) {
	__NodeId = node;
	__Probability = 0.0;
};

void GraphNode::setTransitionProbability(double p) {
	__Probability = p;
};

double GraphNode::getTransitionProbability() {
	return __Probability;
};

int GraphNode::getNodeId() {
	return __NodeId;
};

Graph::Graph(int nodes, double density) {
	__NumberOfNodes = nodes;
	try {
		__GraphMap = new int[nodes * nodes + 5];
		__ProbabilityMap = new double[nodes * nodes + 5];
		__DegreeTable = new int[nodes + 5];
		__NeighborTable = new vector < pair<double, int> >[nodes * nodes + 5];
	} catch (bad_alloc &e) {
		fprintf(stderr, "%s\n", e.what());
	}

	for (int i = 0; i < nodes*nodes; ++i) {
		__GraphMap[i] = 0;
		__ProbabilityMap[i] = 0.0;
	}
	for (int i = 0; i < nodes; ++i)
		__DegreeTable[i] = 0;

	randomEdge(density);
	
	calculateTransitionProbability();

	for (int i = 0; i < nodes; ++i)
		__NeighborTable[i] = findNeighbors(i);
};

Graph::~Graph() {
	delete [] __GraphMap;
	delete [] __ProbabilityMap;
	delete [] __DegreeTable;
	delete [] __NeighborTable;
};

bool Graph::hasNeighbor(int node, int neighbor) {
	return __GraphMap[PositionToIndex(node, neighbor)];
};

void Graph::randomEdge(double density) {
	int count = int(density * __NumberOfNodes * __NumberOfNodes);
	
	// random edges.
	srand((unsigned)time(NULL));
	srand((unsigned)rand());
	while (count) {
		int node_i = rand() % __NumberOfNodes;
		int node_j = rand() % __NumberOfNodes;
		//if (hasNeighbor(node_i, node_j)) continue;
		double r = rand()*1.0 / RAND_MAX;
		if (r < 0.5) continue;

		__GraphMap[PositionToIndex(node_i, node_j)] = 1;
		__GraphMap[PositionToIndex(node_j, node_i)] = 1;
		__DegreeTable[node_i] += 1;
		__DegreeTable[node_j] += 1;
		--count;
	}

	// mark the connection of itself.
	for (int i = 0; i < __NumberOfNodes; ++i)
		__GraphMap[PositionToIndex(i, i)] = 1;
	
	// guarantee the connection of the graph.
	for (int i = 0; i < __NumberOfNodes-1; ++i) {
		__GraphMap[PositionToIndex(i, i+1)] = 1;
		__GraphMap[PositionToIndex(i+1, i)] = 1;
	}
};

void Graph::calculateTransitionProbability() {
	for (int i = 0; i < __NumberOfNodes; ++i) {
		int degree_i = getDegree(i);
		for (int j = 0; j < __NumberOfNodes; ++j) {
			if (!__GraphMap[PositionToIndex(i, j)]) continue;
			int degree_j = getDegree(j);
			__ProbabilityMap[PositionToIndex(i, j)] = (1.0/max(degree_i, degree_j));
		}
	}
	for (int i = 0; i < __NumberOfNodes; ++i) {
		double probability = 1.0;
		for (int j = 0; j < __NumberOfNodes; ++j) {
			if (!__GraphMap[PositionToIndex(i, j)]) continue;
			probability -= __ProbabilityMap[PositionToIndex(i, j)];
		}
		__ProbabilityMap[PositionToIndex(i, i)] = probability;
	}
};

int Graph::getDegree(int node) {
	return __DegreeTable[node];
};

double Graph::getTransitionProbability(int nodei, int nodej) {
	return __ProbabilityMap[PositionToIndex(nodei, nodej)];
};

double Graph::getTransitionProbability(int node) {
	return __ProbabilityMap[PositionToIndex(node, node)];
};

vector < pair <double, int> > Graph::findNeighbors(int node) {
	vector < pair <double, int> > neighbors;
	try {
		for (int k = 0; k < __NumberOfNodes; ++k) {
			neighbors.push_back(make_pair(
				__ProbabilityMap[PositionToIndex(node, k)], k));
		}
		neighbors.push_back(make_pair(
			__ProbabilityMap[PositionToIndex(node, node)], node));
	} catch (bad_alloc &e) {
		fprintf(stderr, "%s\n", e.what());
	}
	//sort(neighbors.begin(), neighbors.end());
	return neighbors;
};

vector < pair <double, int> > Graph::getNeighbors(int node) {
	return __NeighborTable[node];
};

void Graph::dumpGraphDegreeStatistics(char *uri) {
	FILE *dump = fopen(uri, "w");
	for (int i = 0; i < __NumberOfNodes; ++i)
		fprintf(dump, "%d\n", getDegree(i));
	fclose(dump);
};

int Graph::PositionToIndex(int x, int y) {
	return x * __NumberOfNodes + y;
}


void Graph::dumpGraph(char *uri) {
	FILE *fp = fopen(uri, "w");
	for (int i = 0; i < __NumberOfNodes; ++i) {
		for (int j = 0; j < __NumberOfNodes; ++j)
			fprintf(fp, "%d,", __GraphMap[PositionToIndex(i, j)]);
		fprintf(fp, "\n");
	}
	fclose(fp);
};

void Graph::dumpProbability(char *uri) {
	FILE *fp = fopen(uri, "w");
	for (int i = 0; i < __NumberOfNodes; ++i) {
		for (int j = 0; j < __NumberOfNodes; ++j)
			fprintf(fp, "%.2f,", __ProbabilityMap[PositionToIndex(i, j)]);
		fprintf(fp, "\n");
	}
	fclose(fp);
};


}
