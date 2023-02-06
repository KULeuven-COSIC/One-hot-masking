#ifndef MASKEDCOMPARISON_CORON_H
#define MASKEDCOMPARISON_CORON_H

#include <stdint.h>
#include <stddef.h>
#include "params.h"

#ifdef DEBUG
#include <stdio.h>
#include <assert.h>
#endif

void range_compare(uint32_t out[NSHARES], uint32_t CT[NSHARES], const uint32_t public_CT, const uint32_t k, const uint32_t q);
void high_order_compress(uint32_t out[32][NSHARES], uint32_t CT[32][NSHARES], uint32_t q, uint32_t k, uint32_t ell);


uint64_t MaskedComparison_Hybrid(const uint32_t B[NCOEFFS_B][NSHARES], const uint32_t C[NCOEFFS_C][NSHARES],
                          const uint32_t public_B[NCOEFFS_B], const uint32_t public_C[NCOEFFS_C]);


#endif // MASKEDCOMPARISON_H