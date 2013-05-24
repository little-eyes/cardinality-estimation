/*
 * Approximated Cardinality Estimation Project.
 * Author: Jilong Liao (jliao2@utk.edu)
 *
 * RandomCast class and interfaces.
 **/

#ifndef _RANDOMCAST_H
#define _RANDOMCAST_H

#include <vector>
#include <cstdio>
#include <cstdlib>
#include "graph.h"


namespace std {

class RandomCast {

public:
	RandomCast(Graph *graph);
	~RandomCast();
	int absorb(int node, int ttl);

private:
	Graph *__Graph;
	int nextNode(int node);
	FILE *__TTLLogging;
};

/*
 * Customized Random Number Generator.
 */
class RandomGenerator {

public:
	RandomGenerator();
	~RandomGenerator();
	int getRandomInteger();
	double getRandomDouble();
};

}

#endif
