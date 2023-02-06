#ifndef A2C_H
#define A2C_H

#include <stdint.h>
#include <stddef.h>

#ifdef DEBUG
#include <stdio.h>
#include <assert.h>
#endif

void A2C(uint32_t out[NSHARES], uint32_t in[NSHARES], uint32_t form);

#define REG_BITS_L 5
#define REG_BITS 32
#define CARRY_BITS 2

#define MASK(x) ((1<<x)-1)

#endif // A2C_H