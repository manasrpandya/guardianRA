#ifndef FEATURES_H
#define FEATURES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Acquire FEATURE_DIM features into x. Replace implementation with real hooks. */
void features_acquire(float *x, int dim);

#ifdef __cplusplus
}
#endif

#endif /* FEATURES_H */
