#ifndef SECAND_H
#define SECAND_H

#include <stdint.h>
#include <stddef.h>

#ifdef DEBUG
#include <stdio.h>
#include <assert.h>
#endif

void SecAND32(size_t nshares, uint32_t z[nshares], const uint32_t x[nshares], const uint32_t y[nshares]);
void SecAND64(size_t nshares, uint64_t z[nshares], const uint64_t x[nshares], const uint64_t y[nshares]);


#endif // SECAND_H