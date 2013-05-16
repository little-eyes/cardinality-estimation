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

void experiment(FILE *fp, int nodes, double density, 
				int experiments, int ttl, double confidence) {
	map <int, int> records;
	int collisions = 0, probes = 0, d = 0, dmax = 0, dmin = 0;
	int error = -1, xd = 0, xdmax = 0, xdmin = 0, xprobes = 0, xcollisions = 0;
	Graph *graph = new Graph(nodes, density);
	//graph->dumpGraphStatistics();
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
			d = estimator->estimateCardinality(collisions, probes+1);
			dmax = estimator->estimateCardinalityUpperBound(collisions, probes+1);
			dmin = estimator->estimateCardinalityLowerBound(collisions, probes+1);
			if (error < 0 || abs(nodes - d) < error) {
				xd = d;
				xdmax = dmax;
				xdmin = dmin;
				xprobes = probes + 1;
				xcollisions = collisions;
				error = abs(nodes - d);
				// confidence interval satisfied.
				if (error*1.0/nodes < 1 - confidence) {
					fprintf(fp, "%d,%d,%d,%d,%d,%d\n", nodes, xprobes, 
						xcollisions, xd, xdmin, xdmax);
					fflush(fp);
					delete randomcast;
					delete graph;
					delete estimator;
					return;
				}
			}
		}
		++probes;
	}
	d = estimator->estimateCardinality(collisions, probes);
	dmax = estimator->estimateCardinalityUpperBound(collisions, probes);
	dmin = estimator->estimateCardinalityLowerBound(collisions, probes);
	if (error < 0 || abs(nodes - d) < error) {
		xd = d;
		xdmax = dmax;
		xdmin = dmin;
		xprobes = probes;
		xcollisions = collisions;
	}
	fprintf(fp, "%d,%d,%d,%d,%d,%d\n", nodes, xprobes, xcollisions, xd, xdmin, xdmax);
	fflush(fp);
	//display(estimator, collisions, probes);
	delete randomcast;
	delete graph;
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
	for (int n = 0; n < 500; ++n) {
		experiment(data, nodes, density, nodes, nodes, 0.95);
		printf("Running experiment %d ...\n", n);
	}
	fclose(data);
	return 0;
}

