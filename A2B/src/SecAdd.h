#ifndef SECADD_H
#define SECADD_H

#include <stdint.h>
#include <stddef.h>

#ifdef DEBUG
#include <stdio.h>
#include <assert.h>
#endif

void SecAdd(size_t nshares, uint64_t z[nshares], const uint64_t x[nshares], const uint64_t y[nshares]);
void SecAdd32(size_t nshares, uint32_t z[nshares], const uint32_t x[nshares], const uint32_t y[nshares]);
void SecAdd_bitsliced(size_t nshares, size_t nbits, uint32_t z[nshares][nbits], const uint32_t x[nshares][nbits], const uint32_t y[nshares][nbits]);

#endif // SECADD_H