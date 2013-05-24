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
	/* find a destination node from basestation within TTL jumps. */
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
	/* select the next jump based on the given probability distribution. */
	Neighbor *head = __Graph->getNeighbors(node);
	double r = rand()*1.0/RAND_MAX;
	double cumulation = 0.0;
	while (head && head->next()) {
		cumulation += head->getTransitionProbability();
		if (cumulation > r)
			return head->getId();
		head = head->next();
	}
	return head->getId();
};

}
