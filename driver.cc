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
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>

using namespace std;

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

int main(int argc, char **argv) {
	if (argc != 5) exit(1);
	int nodes = atoi(argv[2]);
	double density = atof(argv[4]);
	
	printf("%d %f\n", nodes, density);
	char name[1000];
	memset(name, 0, sizeof(name));
	strcat(name, "out/n_");
	strcat(name, argv[2]);
	strcat(name, "_stats.csv");
	
	printf("%s\n", name);
	FILE *data = fopen(name, "w");
	for (int n = 0; n < 1000; ++n) {
		experiment(data, nodes, density, 5*int(sqrt(nodes)), nodes);
		printf("Running experiment %d ...\n", n);
		fflush(data);
	}
	fclose(data);
	return 0;
}

