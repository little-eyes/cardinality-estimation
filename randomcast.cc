/*
 * Approximated Cardinality Estimation Project.
 * Author: Jilong Liao (jliao2@utk.edu)
 *
 */

#include "randomcast.h"
#include <vector>
#include <algorithm>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <cstdio>

namespace std {

RandomCast::RandomCast(Graph *graph) {
	__Graph = graph;
	srand((unsigned)time(NULL));
	srand((unsigned)rand());
	__TTLLogging = fopen("ttl.csv", "a");
};

RandomCast::~RandomCast() {
	fclose(__TTLLogging);
};

int RandomCast::absorb(int node, int ttl) {
	while (ttl--) {
		int next = nextNode(node);
		if (next == node && node > 0) break;
		node = next;
	}
	fprintf(__TTLLogging, "%d\n", node);
	fflush(__TTLLogging);
	return node;
};

int RandomCast::nextNode(int node) {
	vector < pair<double, int> > neighbors = __Graph->getNeighbors(node);
	//double r = __Distribution(__Generator);
	double r = rand()*1.0/RAND_MAX;
	//printf("%f\n", r);
	double cumulation = 0.0;
	for (int i = 0; i < (int)neighbors.size(); ++i) {
		cumulation += neighbors[i].first;
		if (cumulation > r)
			return neighbors[i].second;
	}
	return neighbors[neighbors.size()-1].second;
};

}
