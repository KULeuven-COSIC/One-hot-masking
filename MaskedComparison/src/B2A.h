#ifndef B2A_H
#define B2A_H

#include <stdint.h>
#include <stddef.h>
#include "params.h"

#ifdef DEBUG
#include <stdio.h>
#include <assert.h>
#endif

void B2A(uint64_t A[NSHARES], uint32_t B[NSHARES]);

#endif // B2A_H