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
#include <cmath>

using namespace std;

static const int GRAPH_NODES = 5000;
static const double GRAPH_DENSITY = 0.3;
static const int MAX_EXPERIMENTS = 500;
static const int TTL = 100;

/*
void display(MaxLikelihoodEstimator *estimator, int collisions, int probes) {
	printf("\n======\n");
	printf("Collisions: %d\nProbes: %d\n", collisions, probes);
	printf("Estimations: d = %d, dmin = %d, dmax = %d\n",
		estimator->estimateCardinality(collisions, probes),
		estimator->estimateCardinalityLowerBound(collisions, probes),
		estimator->estimateCardinalityUpperBound(collisions, probes));
	printf("Ground truth: d = %d\n", GRAPH_NODES);
}
*/

void experiment(FILE *fp, int nodes, double density, int experiments, int ttl) {
	map <int, int> records;
	int collisions = 0, probes = 0, d = 0, dmax = 0, dmin = 0;
	Graph *graph = new Graph(nodes, density);
	//graph->printGraph();
	RandomCast *randomcast= new RandomCast(graph);
	MaxLikelihoodEstimator *estimator = new MaxLikelihoodEstimator();
	for (int iter = 0; iter < experiments; ++iter) {
		int node = randomcast->absorb(0, ttl);
		if (records.find(node) == records.end()) {
			records[node] = 0;
		}
		else {
			records[node] += 1;
			collisions += 1;
			d = estimator->estimateCardinality(collisions, probes);
			dmax = estimator->estimateCardinalityUpperBound(collisions, probes);
			dmin = estimator->estimateCardinalityLowerBound(collisions, probes);
			fprintf(fp, "%d,%d,%d,%d,%d,%d\n", nodes, probes, collisions, d, dmin, dmax);
			//display(estimator, collisions, probes+1);
		}
		++probes;
		//printf("[ ** Probe %d ** ]\n", iter);
	}
	d = estimator->estimateCardinality(collisions, probes);
	dmax = estimator->estimateCardinalityUpperBound(collisions, probes);
	dmin = estimator->estimateCardinalityLowerBound(collisions, probes);
	fprintf(fp, "%d,%d,%d,%d,%d,%d\n", nodes, probes, collisions, d, dmin, dmax);
	//display(estimator, collisions, probes);
	delete graph;
	delete randomcast;
	delete estimator;
}

int main() {
	FILE *data = fopen("stats.csv", "w");
	for (int n = 100; n < 100000; n += 100) {
		experiment(data, n, 0.3, 5*int(sqrt(n)), n/2);
		printf("Running experiment %d ...\n", n/100);
		fflush(data);
	}
	fclose(data);
	return 0;
}

