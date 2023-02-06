#ifndef A2Brotate_H
#define A2Brotate_H

#include <stdint.h>
#include <stddef.h>

#ifdef DEBUG
#include <stdio.h>
#include <assert.h>
#endif

void A2B_inner(uint32_t B[NSHARES], uint32_t A[NSHARES], uint32_t bits);
void A2B_Rotate(size_t NCOEFF, size_t modulus, uint32_t B[NCOEFF][NSHARES], const uint32_t A[NCOEFF][NSHARES]);

#define REG_BITS_L 5
#define REG_BITS 32

#if NSHARES == 2
    #define CARRY_BITS 1
#elif NSHARES <= 4
    #define CARRY_BITS 2
#elif NSHARES <= 8
    #define CARRY_BITS 3
#endif

#define MASK(x) ((1<<x)-1)

#endif // A2C_H