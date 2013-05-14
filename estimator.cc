/*
 * Approximated Cardinality Estimation Project.
 * Author: Jilong Liao (jliao2@utk.edu)
 *
 * The estimator class and interfaces.
 */

#include "estimator.h"
#include <cmath>

namespace std {

MaxLikelihoodEstimator::MaxLikelihoodEstimator() {

}

MaxLikelihoodEstimator::~MaxLikelihoodEstimator() {

}

int MaxLikelihoodEstimator::estimateCardinality(int collisions, int probes) {
	if (collisions == 0) return -1;
	return int(0.5 * probes * probes / collisions) + 1;
}

int MaxLikelihoodEstimator::estimateCardinalityUpperBound(
	int collisions, int probes) {
	if (collisions == 0) return -1;
	return int(probes * probes * (1.0 + 0.5 * collisions + sqrt(1.0 + 
		collisions)) / collisions / collisions) + 1;
}

int MaxLikelihoodEstimator::estimateCardinalityLowerBound(
	int collisions, int probes) {
	if (collisions == 0) return -1;
	return int(probes * probes * (1.0 + 0.5 * collisions - sqrt(1.0 +
		collisions)) / collisions / collisions) + 1;
}

}
