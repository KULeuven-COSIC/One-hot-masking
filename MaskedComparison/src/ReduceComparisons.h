#ifndef REDUCECOMPARISONS_H
#define REDUCECOMPARISONS_H

#include <stdint.h>
#include <stddef.h>
#include "params.h"

#ifdef DEBUG
#include <stdio.h>
#include <assert.h>
#endif

struct uint96_t
{
    uint64_t LSB[NSHARES];
    uint32_t MSB[NSHARES];
};


void ReduceComparisons(uint64_t E[NSHARES], const uint64_t D[NCOEFFS_B + NCOEFFS_C][NSHARES]);

void ReduceComparisons_GF(struct uint96_t *E, uint32_t BC_Bitsliced[ SIMPLECOMPBITS ][NSHARES]);

#endif // REDUCECOMPARISONS_H