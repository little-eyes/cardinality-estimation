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
#include <ctime>

using namespace std;

const int GRAPH_GENERATION = 0;
const int RANDOM_CAST = 1;


/*
 * The experiment main body. Each experiment will continue sending probes
 * until the error bound reached or the maximum number of probes reached.
 * 
 * The maximum number of probes is the number of nodes (no need to send
 * more than it).
 */
void experiment(FILE *fp, int nodes, double density, 
				int experiments, int ttl, double confidence,
				FILE *measure=NULL) {
	map <int, int> records;
	int collisions = 0, probes = 0, d = 0, dmax = 0, dmin = 0;
	int error = -1, xd = 0, xdmax = 0, xdmin = 0, xprobes = 0, xcollisions = 0;
	
	// create Graph, RandomCast and Estimator classes.
	clock_t start = clock();
	Graph *graph = new Graph(nodes, density);
	clock_t end = clock();
	fprintf(measure, "%d,%f\n", 0, (end - start)*1.0/CLOCKS_PER_SEC);
	fflush(measure);
	//graph->dumpGraphDegreeStatistics("out/degree.csv");
	RandomCast *randomcast= new RandomCast(graph);
	MaxLikelihoodEstimator *estimator = new MaxLikelihoodEstimator();

	start = clock();
	for (int iter = 0; iter < experiments; ++iter) {
		int node = randomcast->absorb(0, ttl); // send probe from 0 (basestation) to others.
		fflush(measure);
		if (records.find(node) == records.end()) {
			records[node] = 0;
		}
		else {
			// collision happens.
			records[node] += 1;
			collisions += 1;
			// make estimations.
			d = estimator->estimateCardinality(collisions, probes+1);
			dmax = estimator->estimateCardinalityUpperBound(collisions, probes+1);
			dmin = estimator->estimateCardinalityLowerBound(collisions, probes+1);
			// update the best values.
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
					end = clock();
					fprintf(measure, "%d,%f\n", 1, (end - start)*1.0/CLOCKS_PER_SEC);
					fflush(measure);
					// deallocate memory.
					start = clock();
					delete randomcast;
					delete graph;
					delete estimator;
					end = clock();
					fprintf(measure, "%d,%f\n", 3, (end - start)*1.0/CLOCKS_PER_SEC);
					fflush(measure);
					return;
				}
			}
		}
		++probes;
	}
	end = clock();
	fprintf(measure, "%d,%f\n", 1, (end - start)*1.0/CLOCKS_PER_SEC);
	fflush(measure);

	// max number of experiments reach, record the best results it can get. 
	start = clock();
	d = estimator->estimateCardinality(collisions, probes);
	dmax = estimator->estimateCardinalityUpperBound(collisions, probes);
	dmin = estimator->estimateCardinalityLowerBound(collisions, probes);
	end = clock();
	fprintf(measure, "%d,%f\n", 2, (end - start)*1.0/CLOCKS_PER_SEC);

	if (error < 0 || abs(nodes - d) < error) {
		xd = d;
		xdmax = dmax;
		xdmin = dmin;
		xprobes = probes;
		xcollisions = collisions;
	}
	fprintf(fp, "%d,%d,%d,%d,%d,%d\n", nodes, xprobes, xcollisions, xd, xdmin, xdmax);
	fflush(fp);
	
	// deallocate memory.
	start = clock();
	delete graph;
	delete randomcast;
	delete estimator;
	end = clock();
	fprintf(measure, "%d,%f\n", 3, (end - start)*1.0/CLOCKS_PER_SEC);
	fflush(measure);
}

int main(int argc, char **argv) {
	if (argc != 5) exit(1);
	int nodes = atoi(argv[2]);
	double density = atof(argv[4]);
	
	char name[1000];
	memset(name, 0, sizeof(name));
	strcat(name, "out/n_");
	strcat(name, argv[2]);
	strcat(name, "_stats.csv");
	
	FILE *data = fopen(name, "w");
	strcat(name, ".perf");
	FILE *measure = fopen(name, "w");
	for (int n = 0; n < 500; ++n) {
		clock_t s = clock();
		experiment(data, nodes, density, nodes, nodes, 0.95, measure);
		clock_t e = clock();
		printf("Experiment %d ... done!\n", n);
		fprintf(measure, "%d,%f\n", -1, (e-s)*1.0/CLOCKS_PER_SEC);
	}
	fclose(data);
	fclose(measure);
	return 0;
}

