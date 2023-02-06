#ifndef SECMULT_H
#define SECMULT_H

#include <stdint.h>
#include <stddef.h>

#ifdef DEBUG
#include <stdio.h>
#include <assert.h>
#endif

void secMult(uint32_t nshares, uint32_t q, uint32_t out[nshares], uint32_t A[nshares], uint32_t B[nshares]);
uint32_t uncompress(uint32_t x, uint32_t compressto, uint32_t q);
void randomq(uint32_t out[2], uint32_t q);

#endif