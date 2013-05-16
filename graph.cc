/*
 * Approximated Cardinality Estimation Project
 * Author: Jilong Liao (jliao2@utk.edu)
 *
 **/

#include "graph.h"
#include <vector>
#include <set>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <algorithm>
#include <utility>

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
	//printf("%d %.2f\n", nodes, density);
	__NumberOfNodes = nodes;
	for (int i = 0; i < __NumberOfNodes; ++i) {
		vector <GraphNode *> row;
		__GraphMap.push_back(row);
		GraphNode *nd = new GraphNode(i);
		__Nodes.push_back(nd);
	}

	// make sure the connectivity of the graph.
	for (int u = 0; u < __NumberOfNodes-1; ++u) {
		// insert itself.
		GraphNode *nd = new GraphNode(u+1);
		__GraphMap[u].push_back(nd);
	}
	
	randomEdge(density);
	
	calculateTransitionProbability();
};

Graph::~Graph() {
	// clear the __GraphMap.
	for (int i = 0; i < __NumberOfNodes; ++i) {
		for (int j = __GraphMap[i].size()-1; j >= 0; --j) {
			GraphNode *nd = __GraphMap[i][j];
			__GraphMap[i].pop_back();
			delete nd;
		}
	}
	// clear the __Nodes.
	for (int i = __NumberOfNodes-1; i >= 0; --i) {
		GraphNode *nd = __Nodes[i];
		__Nodes.pop_back();
		delete nd;
	}
};

bool Graph::hasNeighbor(int node, int neighbor) {
	for (int i = 0; i < __GraphMap[node].size(); ++i) 
		if (__GraphMap[node][i]->getNodeId() == neighbor) return true;
	return false;
};

void Graph::randomEdge(double density) {
	int count = int(density * __NumberOfNodes * __NumberOfNodes);
	//printf("%d\n", count);
	srand(time(NULL));
	while (count) {
		int node_i = rand() % __NumberOfNodes;
		int node_j = rand() % __NumberOfNodes;
		if (hasNeighbor(node_i, node_j)) continue;
		double r = rand()*1.0 / RAND_MAX;
		//printf("%.2f\n", r);
		if (r >= 0.5) {
			GraphNode *nd_j = new GraphNode(node_j);
			__GraphMap[node_i].push_back(nd_j);
			GraphNode *nd_i = new GraphNode(node_i);
			__GraphMap[node_j].push_back(nd_i);
			--count;
		}
	}
};

void Graph::calculateTransitionProbability() {
	for (int i = 0; i < __NumberOfNodes; ++i) {
		int degree_i = getDegree(i);
		for (int j = 0; j < __GraphMap[i].size(); ++j) {
			int degree_j = getDegree(__GraphMap[i][j]->getNodeId());
			__GraphMap[i][j]->setTransitionProbability(1.0/max(degree_i, degree_j));
		}
	}
	for (int i = 0; i < __NumberOfNodes; ++i) {
		double probability = 1.0;
		for (int j = 0; j < __GraphMap[i].size(); ++j)
				probability -= __GraphMap[i][j]->getTransitionProbability();
		__Nodes[i]->setTransitionProbability(probability);
	}
};

int Graph::getDegree(int node) {
	return __GraphMap[node].size();
};

double Graph::getTransitionProbability(int nodei, int nodej) {
	for (int k = 0; k < __GraphMap[nodei].size(); ++k)
		if (__GraphMap[nodei][k]->getNodeId() == nodej)
			return __GraphMap[nodei][k]->getTransitionProbability();
	return -1.0;
};

double Graph::getTransitionProbability(int node) {
	for (int k = 0; k < __Nodes.size(); ++k)
		if (__Nodes[k]->getNodeId() == node)
			return __Nodes[k]->getTransitionProbability();
	return -1.0;
};

vector < pair <double, int> > Graph::getNeighbors(int node) {
	vector < pair <double, int> > neighbors;
	for (int k = 0; k < __GraphMap[node].size(); ++k) {
		neighbors.push_back(make_pair(
			__GraphMap[node][k]->getTransitionProbability(), 
			__GraphMap[node][k]->getNodeId()));
	}
	neighbors.push_back(make_pair(
		__Nodes[node]->getTransitionProbability(),
		__Nodes[node]->getNodeId()));
	sort(neighbors.begin(), neighbors.end());
	return neighbors;
};

void Graph::dumpGraphStatistics() {
	FILE *dump = fopen("out/graph.csv", "a");
	for (int i = 0; i < __NumberOfNodes; ++i)
		fprintf(dump, "%d\n", getDegree(__Nodes[i]->getNodeId()));
	fclose(dump);
};

/*
void Graph::printGraph() {
	//for (int i = 0; i < __NumberOfNodes; ++i) {
	//	for (int j = 0; j < __NumberOfNodes; ++j)
	//		printf("%d ", __GraphMap[i][j]);
	//	printf("\n");
	//}
	printf("\n=======\n");
	FILE *fp = fopen("graph.csv", "w");
	for (int i = 0; i < __NumberOfNodes; ++i) {
		for (int j = 0; j < __NumberOfNodes; ++j)
			fprintf(fp, "%.2f ", __TransitionProbability[i][j]);
		fprintf(fp, "\n");
	}
	fclose(fp);
};
*/

}
