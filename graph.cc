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

/*
 * GraphNode class. 
 */
GraphNode::GraphNode(int node, int size) {
	/* memory allocation and initialization. */
	__NodeId = node;
	__Neighbors = NULL;
	__LastNeighbor = NULL;
	try {
		__NeighborBitMap = new int[size/sizeof(int)];
	} catch (bad_alloc &e) {
		fprintf(stderr, "%s\n", e.what());
	}
	for (int i = 0; i < size/sizeof(int); ++i)
		__NeighborBitMap[i] = false;
	__Degree = 0;
};

GraphNode::~GraphNode() {
	/* memory dellocation. */
	Neighbor *head = __Neighbors;
	while (head && head->next()) {
		Neighbor *next = head->next();
		delete head;
		head = next;
	}
	delete __NeighborBitMap;
};

int GraphNode::getNodeId() {
	/* obtain the node ID. */
	return __NodeId;
};

void GraphNode::addNeighbor(int node) {
	/* add a neighbor to a node, but the edge transition probability is not ready yet.*/
	Neighbor *neighbor = new Neighbor(node, 0.0);
	if (!__Neighbors) __Neighbors = neighbor;
	if (!__LastNeighbor) __LastNeighbor = neighbor;
	else {
		__LastNeighbor->append(neighbor);
		__LastNeighbor = __LastNeighbor->next();
	}
	++__Degree;
	__NeighborBitMap[node/sizeof(int)] |= (1 << (node % sizeof(int)));
};

Neighbor *GraphNode::getNeighbors() {
	/* return the pointer to all neighbors. */
	return __Neighbors;
};

int GraphNode::getDegree() {
	/* return the node's degree. */
	return __Degree;
};

void GraphNode::setTransitionProbability(int node, double probability) {
	/* calculate the transition probability of edge (graphnode, node). */
	Neighbor *head = __Neighbors;
	while (head) {
		if (head->getId() == node) {
			head->setTransitionProbability(probability);
			return;
		}
		head = head->next();
	}
};

bool GraphNode::hasNeighbor(int node) {
	/* return if graphnode has a neighbor recorded in the bitmap. */
	return (__NeighborBitMap[node/sizeof(int)] >> (node % sizeof(int))) & 1;
};

/*
 * Neighbors class.
 */
Neighbor::Neighbor(int node, double probability) {
	/* no dynmaic memory allocated. */
	__NodeId = node;
	__Probability = probability;
	__Next = NULL;
};

Neighbor::~Neighbor() {

};

void Neighbor::append(Neighbor *neighbor) {
	__Next = neighbor;
};

int Neighbor::getId() {
	return __NodeId;
};

double Neighbor::getTransitionProbability() {
	return __Probability;
};

Neighbor *Neighbor::next() {
	return __Next;
};

void Neighbor::setTransitionProbability(double probability) {
	__Probability = probability;
};

/*
 * Graph class.
 */
Graph::Graph(int nodes, double density) {
	/* allocate memory, generate random graph and calculate probability.*/
	__NumberOfNodes = nodes;
	try {
		__GraphMap = new GraphNode *[nodes];
	} catch (bad_alloc &e) {
		fprintf(stderr, "%s\n", e.what());
	}

	for (int i = 0; i < nodes; ++i) {
		__GraphMap[i] = new GraphNode(i, nodes);
	}

	randomEdge(density);
	
	calculateTransitionProbability();
};

Graph::~Graph() {
	/* dellocate memory. */
	delete [] __GraphMap;
};

bool Graph::hasNeighbor(int node, int neighbor) {
	/* API to check whether neighor exists. */
	return __GraphMap[node]->hasNeighbor(neighbor);
};

void Graph::randomEdge(double density) {
	/* Generate random edge by randomly selecting edges. */
	int count = int(density * __NumberOfNodes * __NumberOfNodes);
	
	// random edges.
	srand((unsigned)time(NULL));
	srand((unsigned)rand());
	while (count) {
		--count;
		int node_i = rand() % __NumberOfNodes;
		int node_j = rand() % __NumberOfNodes;
		if (hasNeighbor(node_i, node_j)) continue;
		double r = rand()*1.0 / RAND_MAX;
		if (r < 0.5) continue;

		__GraphMap[node_i]->addNeighbor(node_j);
		__GraphMap[node_j]->addNeighbor(node_i);
	}

	// mark the connection of itself.
	for (int i = 0; i < __NumberOfNodes; ++i) {
		if (hasNeighbor(i, i)) continue;
		__GraphMap[i]->addNeighbor(i);
	}
	
	// guarantee the connection of the graph.
	for (int i = 0; i < __NumberOfNodes-1; ++i) {
		if (hasNeighbor(i, i+1)) continue;
		__GraphMap[i]->addNeighbor(i+1);
	}
};

void Graph::calculateTransitionProbability() {
	/* calculate the transition probability, 
	 * p(i, j) = 1.0/max(degree(i), degree(j))
	 * p(i) = 1.0 - sum(p(i, j)). */

	// calculate p(i, j).
	for (int i = 0; i < __NumberOfNodes; ++i) {
		int degree_i = getDegree(i);
		Neighbor *head = __GraphMap[i]->getNeighbors();
		while (head) {
			if (head->getId() != i) {
				int degree_j = getDegree(head->getId());
				head->setTransitionProbability(1.0/max(degree_i, degree_j));
			}
			head = head->next();
		}
	}
	// calculate p(i).
	for (int i = 0; i < __NumberOfNodes; ++i) {
		double probability = 1.0;
		Neighbor *head = __GraphMap[i]->getNeighbors();
		Neighbor *target = NULL;
		while (head) {
			if (head->getId() != i)
				probability -= head->getTransitionProbability();
			else
				target = head;
			head = head->next();
		}
		target->setTransitionProbability(probability);
	}
};

int Graph::getDegree(int node) {
	/* API: obtain the degrees. */
	return __GraphMap[node]->getDegree();
};

double Graph::getTransitionProbability(int nodei, int nodej) {
	/* API: obtain the transition probability of p(i, j). */
	Neighbor *head = __GraphMap[nodei]->getNeighbors();
	while (head) {
		if (head->getId() == nodej)
			return head->getTransitionProbability();
		head = head->next();
	}
	return 0.0;
};

double Graph::getTransitionProbability(int node) {
	/* API: obtain the transition probability of p(i). */
	return getTransitionProbability(node, node);
};

Neighbor *Graph::getNeighbors(int node) {
	/* API: obtain the neighbors list pointer. */
	return __GraphMap[node]->getNeighbors();
};

void Graph::dumpGraphDegreeStatistics(char *uri) {
	/* API: for statistics purpose only. */
	FILE *fp = fopen(uri, "w");
	for (int i = 0; i < __NumberOfNodes; ++i)
		fprintf(fp, "%d\n", __GraphMap[i]->getDegree());
	fclose(fp);
};

}
