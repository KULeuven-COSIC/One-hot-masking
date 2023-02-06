#ifndef A2B_H
#define A2B_H

#include <stdint.h>
#include <stddef.h>
#include "params.h"

#ifdef DEBUG
#include <stdio.h>
#include <assert.h>
#endif

void A2B(size_t nshares, uint64_t B[nshares], const uint64_t A[nshares]);
void A2B32(size_t nshares, uint32_t B[nshares], const uint32_t A[nshares]);
void A2B_bitsliced(size_t nshares, size_t nbits, uint32_t B[32][nshares], const uint32_t A[32][nshares]);
void A2B_keepbitsliced(size_t nshares, uint32_t ncoefsb, uint32_t compressfrom_b, uint32_t compressto_b, uint32_t ncoefsc, uint32_t compressfrom_c, uint32_t compressto_c, uint32_t out[SIMPLECOMPBITS][NSHARES], const uint32_t Bp[ncoefsb][NSHARES], const uint32_t Cp[ncoefsc][NSHARES]);

#endif // A2B_H