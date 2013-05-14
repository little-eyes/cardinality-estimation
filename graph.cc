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
#include <utility>

namespace std {

Graph::Graph(int nodes, double density) {
	//printf("%d %.2f\n", nodes, density);
	__NumberOfNodes = nodes;
	for (int i = 0; i < __NumberOfNodes; ++i) {
		vector <int> row;
		vector <double> prow;
		for (int j = 0; j < __NumberOfNodes; ++j) {
			row.push_back(0);
			prow.push_back(0.0);
		}
		__GraphMap.push_back(row);
		__TransitionProbability.push_back(prow);
	}

	for (int i = 0; i < __NumberOfNodes; ++i)
		__GraphMap[i][i] = 1;
	
	randomEdge(density);
	
	for (int i = 0; i < __NumberOfNodes-1; ++i) 
		__GraphMap[i][i+1] = __GraphMap[i+1][i] = 1;
	
	calculateTransitionProbability();
};

void Graph::randomEdge(double density) {
	int count = int(density * __NumberOfNodes * __NumberOfNodes);
	//printf("%d\n", count);
	srand(time(NULL));
	while (count) {
		int node_i = rand() % __NumberOfNodes;
		int node_j = rand() % __NumberOfNodes;
		if (node_i == node_j || __GraphMap[node_i][node_j] == 1) continue;
		double r = rand()*1.0 / RAND_MAX;
		//printf("%.2f\n", r);
		if (r >= 0.5) {
			__GraphMap[node_i][node_j] = 1;
			__GraphMap[node_j][node_i] = 1;
			--count;
		}
	}
};

void Graph::calculateTransitionProbability() {
	for (int i = 0; i < __NumberOfNodes; ++i) {
		for (int j = 0; j < __NumberOfNodes; ++j) {
			if (i == j || __GraphMap[i][j] == 0) continue;
			int degree_i = getDegree(i);
			int degree_j = getDegree(j);
			__TransitionProbability[i][j] = 1.0/max(degree_i, degree_j);
		}
	}
	for (int i = 0; i < __NumberOfNodes; ++i) {
		__TransitionProbability[i][i] = 1.0;
		for (int j = 0; j < __NumberOfNodes; ++j) {
			if (i == j) continue;
			if (__GraphMap[i][j] == 1)
				__TransitionProbability[i][i] -= __TransitionProbability[i][j];
		}
	}
};

int Graph::getDegree(int node) {
	int degree = 0;
	for (int i = 0; i < __NumberOfNodes; ++i)
		degree += __GraphMap[node][i];
	return degree;
};

double Graph::getTransitionProbability(int nodei, int nodej) {
	return __TransitionProbability[nodei][nodej];
};

double Graph::getTransitionProbability(int nodei) {
	return __TransitionProbability[nodei][nodei];
};

vector < pair <double, int> > Graph::getNeighbors(int node) {
	vector < pair <double, int> > neighbors;
	for (int i = 0; i < __NumberOfNodes; ++i) {
		if (__GraphMap[node][i] == 1)
			neighbors.push_back(make_pair(__TransitionProbability[node][i], i));
	}
	sort(neighbors.begin(), neighbors.end());
	return neighbors;
};

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

}
