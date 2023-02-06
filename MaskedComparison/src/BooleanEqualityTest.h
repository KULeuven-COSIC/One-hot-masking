#ifndef BOOLEANEQUALITYTEST_H
#define BOOLEANEQUALITYTEST_H

#include <stdint.h>
#include <stddef.h>
#include "params.h"
#include "ReduceComparisons.h"

#ifdef DEBUG
#include <stdio.h>
#include <assert.h>
#endif

uint32_t BooleanEqualityTest(uint64_t E[NSHARES]);

uint32_t BooleanEqualityTest_GF(struct uint96_t B);

uint32_t BooleanEqualityTest_Simple(uint32_t B[SIMPLECOMPBITS][NSHARES], uint32_t len);

uint32_t BooleanEqualityTest_Simple_NBS(uint32_t B[SIMPLECOMPBITS][NSHARES], uint32_t len);


#endif // BOOLEANEQUALITYTEST_H