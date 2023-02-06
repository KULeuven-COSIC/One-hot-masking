#ifndef A2Btable_H
#define A2Btable_H

#include <stdint.h>
#include <stddef.h>

#ifdef DEBUG
#include <stdio.h>
#include <assert.h>
#endif

void A2B_table(size_t NCOEFF, size_t modulus, uint32_t B[NCOEFF][NSHARES], const uint32_t A[NCOEFF][NSHARES]);

#define REG_BITS_L 5
#define REG_BITS 32

#endif // A2C_H