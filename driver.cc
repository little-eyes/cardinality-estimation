/*
 * Approximated Cardinality Estimation Project.
 * Author: Jilong Liao (jliao2@utk.edu)
 *
 * Driver file.
 */


#include "graph.h"
#include "randomcast.h"
#include "estimator.h"
#include <cstdio>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <unistd.h>
#include <pthread.h>

using namespace std;

static const int GRAPH_NODES = 5000;
static const double GRAPH_DENSITY = 0.3;
static const int MAX_EXPERIMENTS = 500;
static const int TTL = 100;

void display(MaxLikelihoodEstimator *estimator, int collisions, int probes) {
	printf("\n======\n");
	printf("Collisions: %d\nProbes: %d\n", collisions, probes);
	printf("Estimations: d = %d, dmin = %d, dmax = %d\n",
		estimator->estimateCardinality(collisions, probes),
		estimator->estimateCardinalityLowerBound(collisions, probes),
		estimator->estimateCardinalityUpperBound(collisions, probes));
	printf("Ground truth: d = %d\n", GRAPH_NODES);
}

void *experiment(void *) {
	map <int, int> records;
	int collisions = 0, probes = 0;
	//printf("%d %.2f\n", GRAPH_NODES, GRAPH_DENSITY);	
	Graph *graph = new Graph(GRAPH_NODES, GRAPH_DENSITY);
	graph->printGraph();
	RandomCast *randomcast= new RandomCast(graph);
	MaxLikelihoodEstimator *estimator = new MaxLikelihoodEstimator();
	for (int iter = 0; iter < MAX_EXPERIMENTS; ++iter) {
		int node = randomcast->absorb(0, TTL);
		//printf("%d\n", node);
		if (records.find(node) == records.end()) {
			records[node] = 0;
		}
		else {
			records[node] += 1;
			collisions += 1;
			display(estimator, collisions, probes+1);
		}
		++probes;
		printf("[ ** Probe %d ** ]\n", iter);
	}
	display(estimator, collisions, probes);
	delete graph;
	delete randomcast;
	delete estimator;
}

int main() {
	pthread_t thread;
	pthread_create(&thread, NULL, &experiment, NULL);
	void *result;
	pthread_join(thread, &result);
	return 0;
}

