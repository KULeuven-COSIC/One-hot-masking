#ifndef BITMASK_H
#define BITMASK_H

#include <stdint.h>
#include <stddef.h>

static uint64_t bit_mask(size_t nbits)
{
    return ((uint64_t)(1 << nbits) - 1);
}

#endif // BITMASK_H