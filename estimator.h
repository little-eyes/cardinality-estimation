/*
 * Approximated Cardinality Estimation Project.
 * Author: Jilong Liao (jliao2@utk.edu)
 *
 * Driver class and interfaces.
 */

#ifndef _ESTIMATOR_H
#define _ESTIMATOR_H

#include "graph.h"
#include "randomcast.h"

namespace std {

class MaxLikelihoodEstimator {

public:
	MaxLikelihoodEstimator();
	~MaxLikelihoodEstimator();
	int estimateCardinality(int collisions, int probes);
	int estimateCardinalityUpperBound(int collisions, int probes);
	int estimateCardinalityLowerBound(int collisions, int probes);

};

}

#endif
