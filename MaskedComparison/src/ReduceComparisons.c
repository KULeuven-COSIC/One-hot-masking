#include "ReduceComparisons.h"
#include "randombytes.h"
#include "params.h"

void ReduceComparisons(uint64_t E[NSHARES], const uint64_t D[NCOEFFS_B + NCOEFFS_C][NSHARES])
{
    for (size_t j = 0; j < NSHARES; j++)
    {
        E[j] = 0;
    }

    for (size_t i = 0; i < (NCOEFFS_B + NCOEFFS_C); i++)
    {
        uint64_t R = random_uint64_in();

        #ifdef DEBUG
        uint64_t D_unmasked = 0;
        #endif

        for (size_t j = 0; j < NSHARES; j++)
        {
            E[j] += R * D[i][j];

            #ifdef DEBUG
            D_unmasked += D[i][j];
            #endif
        }
    }
}

void ReduceComparisons_GF(struct uint96_t *E, uint32_t BC_Bitsliced[ SIMPLECOMPBITS ][NSHARES])
{
    uint32_t biti;
    uint64_t tmp;

    for (size_t i = 0; i < NSHARES; i++)
    {
        for (size_t j = 0; j < NSHARES; j++)
        {
            E->MSB[i] = 0;
            E->LSB[i] = 0;
        }
    }

    for (size_t i = 0; i < SIMPLECOMPBITS; i++)
    {
        uint64_t R = random_uint64_in();

        for (size_t j = 0; j < NSHARES; j++) 
        {
            for (size_t k = 0; k < 32; k++)
            {
                biti = (BC_Bitsliced[i][j] >> k) & 1 ;
                tmp = R * biti;
                E->LSB[j] ^= tmp << k ;
                E->MSB[j] ^= tmp >> (64 - k) ;
            }
        }
    }
}