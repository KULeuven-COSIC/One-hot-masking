#ifndef A2B_H
#define A2B_H

#include <stdint.h>
#include <stddef.h>

#ifdef DEBUG
#include <stdio.h>
#include <assert.h>
#endif

void A2B_NB(size_t NCOEFF, size_t nbits, uint32_t B[NCOEFF][NSHARES], const uint32_t A[NCOEFF][NSHARES]);
void A2B(size_t nshares, uint64_t B[nshares], const uint64_t A[nshares]);
void A2B_bitsliced(size_t nshares, size_t nbits, uint32_t B[32][nshares], const uint32_t A[32][nshares]);

#endif // A2B_H