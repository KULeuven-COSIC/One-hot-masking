#ifndef MASKEDCOMPARISON_H
#define MASKEDCOMPARISON_H

#include <stdint.h>
#include <stddef.h>
#include "params.h"

#ifdef DEBUG
#include <stdio.h>
#include <assert.h>
#endif


uint64_t MaskedComparison_Rotate(const uint32_t B[NCOEFFS_B][NSHARES], const uint32_t C[NCOEFFS_C][NSHARES],
                          const uint32_t public_B[NCOEFFS_B], const uint32_t public_C[NCOEFFS_C]);

uint64_t MaskedComparison_Arith(const uint32_t B[NCOEFFS_B][NSHARES], const uint32_t C[NCOEFFS_C][NSHARES],
                          const uint32_t public_B[NCOEFFS_B], const uint32_t public_C[NCOEFFS_C]);

uint64_t MaskedComparison_Simple(const uint32_t B[NCOEFFS_B][NSHARES], const uint32_t C[NCOEFFS_C][NSHARES],
                          const uint32_t public_B[NCOEFFS_B], const uint32_t public_C[NCOEFFS_C]);

uint64_t MaskedComparison_Simple_NBS(const uint32_t B[NCOEFFS_B][NSHARES], const uint32_t C[NCOEFFS_C][NSHARES],
                          const uint32_t public_B[NCOEFFS_B], const uint32_t public_C[NCOEFFS_C]);

uint64_t MaskedComparison_Simple_NBSO(const uint32_t B[NCOEFFS_B][NSHARES], const uint32_t C[NCOEFFS_C][NSHARES],
                          const uint32_t public_B[NCOEFFS_B], const uint32_t public_C[NCOEFFS_C]);

uint64_t MaskedComparison_GF(const uint32_t B[NCOEFFS_B][NSHARES], const uint32_t C[NCOEFFS_C][NSHARES],
                          const uint32_t public_B[NCOEFFS_B], const uint32_t public_C[NCOEFFS_C]);

uint64_t MaskedComparison_HybridSimple(const uint32_t B[NCOEFFS_B][NSHARES], const uint32_t C[NCOEFFS_C][NSHARES],
                          const uint32_t public_B[NCOEFFS_B], const uint32_t public_C[NCOEFFS_C]);

#endif // MASKEDCOMPARISON_H