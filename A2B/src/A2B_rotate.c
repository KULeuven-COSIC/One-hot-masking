#include "A2B_rotate.h"
#include <stdbool.h>
#include "hal.h"
#include "randombytes.h"
#include <string.h>

#ifdef DEBUG
void binprintf(uint32_t v)
{
    uint32_t mask=1<<((sizeof(uint32_t)<<3)-1);
    while(mask) {
        printf("%d", (v&mask ? 1 : 0));
        mask >>= 1;
    }
    printf("\n");
}

void binprintshared(uint32_t *v)
{
    uint32_t v2 = 0;
    for(uint32_t sh=0; sh < NSHARES; sh++)
    {
        v2 ^= v[sh];
    }
    uint32_t mask=1<<((sizeof(uint32_t)<<3)-1);
    while(mask) {
        printf("%d", (v2&mask ? 1 : 0));
        mask >>= 1;
    }
    printf(" = %d", v2);
    printf("\n");
}
#endif

inline static uint32_t ROT(uint32_t R, uint32_t shift)
{
    R = ( (R << shift) | (R >> (REG_BITS - shift)) );
    return R;
}

static void ROT_remask(uint32_t reg[NSHARES], uint32_t in[NSHARES])
{
    for(uint32_t sh_in=0; sh_in<NSHARES; sh_in++)
    {
        // rotate + reshare
        reg[NSHARES-1] =  ROT(reg[NSHARES-1], in[sh_in]); // start with last share; so you can already do reshare in loop
        for(uint32_t sh_reg=0; sh_reg < NSHARES-1; sh_reg++)
        {
            reg[sh_reg] =  ROT(reg[sh_reg], in[sh_in]); 

            uint32_t r = random_uint32_in();
            reg[sh_reg] = reg[sh_reg] ^ r;
            reg[NSHARES-1] = reg[NSHARES-1] ^ r;
        }
    }
}

static void filter(uint32_t Bpart[NSHARES], uint32_t reg[NSHARES], uint32_t INP_BITS)
{
    uint32_t reg2;

    // uint32_t BOOLEANMASK[REG_BITS_L+1] =  {0b10101010101010101010101010101010, 0b11001100110011001100110011001100, 0b11110000111100001111000011110000, 0b11111111000000001111111100000000, 0b11111111111111110000000000000000};

    for(uint32_t sh=0; sh < NSHARES; sh++)
    {
        reg2 = reg[sh];

        // CALCULATE BOOLEAN SHARING
        for(uint32_t i=INP_BITS; i < REG_BITS_L; i++)
        {
            reg2 ^= reg2 >> (1 << i);
        }

        Bpart[sh] = 0;
        for(uint32_t i=0; i < ((uint32_t)1 << INP_BITS); i++)
        {
            uint32_t bit = (reg2 >> i) & 1;
            Bpart[sh] ^= (bit * i);
        }

        // other way to do the above two loops, but was slower
        // Bpart[sh] = 0;
        // for(uint32_t i=0; i < INP_BITS; i++)
        // {
        //     tmp = __builtin_parity(reg[sh] & BOOLEANMASK[i]);
        //     Bpart[sh] ^= tmp << i;
        // }
    }

    uint32_t filter = 1;
    for(uint32_t i= (1 << INP_BITS); i < REG_BITS; i=i<<1)
    {
        filter += filter << i;
    }
    uint32_t filter2 = ((1 << (1<<CARRY_BITS)) - 1);
    uint32_t shift = (1<<INP_BITS) - 1; 
    
    for(uint32_t sh=0; sh < NSHARES; sh++)
    {
        // REGULAR FILTER
        // take care of positions that are allowed to be non-zero
        for(uint32_t i=0; i < INP_BITS; i++)
        {
            reg[sh] ^=  reg[sh] >> (1 << i);
        }
        reg[sh] = reg[sh] & filter;

        // take care of carries
        for(uint32_t i=shift; i < REG_BITS; i=i<<1)
        {
            reg[sh] ^= reg[sh] >> i;
        }
        reg[sh] = reg[sh] & filter2;
    }
}

void A2B_inner(uint32_t B[NSHARES], uint32_t A[NSHARES], uint32_t bits)
{

    uint32_t reg[NSHARES];
    uint32_t Bpart[NSHARES];
    uint32_t INP_BITS = REG_BITS_L - CARRY_BITS;

    PROFILE_STEP_INIT();

    // prepare register
    reg[0] = 1;
    B[0] = 0;
    for (size_t sh = 1; sh < NSHARES; sh++)
    {
        reg[sh] = 0;
        B[sh] = 0;
    }

    // start the loop 
    uint32_t loc = 0;
    for(;loc<bits-REG_BITS_L;loc+=INP_BITS)
    {
        PROFILE_STEP_START();
        // PROFILE_STEP_START();
        uint32_t A2[NSHARES];
        

        for(uint32_t sh=0; sh < NSHARES; sh++)
        {
            A2[sh] = A[sh] & MASK(INP_BITS);
            A[sh] = A[sh] >> INP_BITS;
        }
        PROFILE_STEP_STOP(10);

        PROFILE_STEP_START();
        ROT_remask(reg, A2);
        PROFILE_STEP_STOP(11);

        PROFILE_STEP_START();
        filter(Bpart, reg, INP_BITS);

        for(uint32_t sh=0; sh < NSHARES; sh++)
        {
            B[sh] |= Bpart[sh] << loc;
        }
        PROFILE_STEP_STOP(12);
    }

    PROFILE_STEP_START();
    ROT_remask(reg, A);
    PROFILE_STEP_STOP(21);

    PROFILE_STEP_START();
    //  final operation is faster this way
    INP_BITS = bits - loc;
    uint32_t BOOLEANMASK[REG_BITS_L+1] =  {0b10101010101010101010101010101010, 0b11001100110011001100110011001100, 0b11110000111100001111000011110000, 0b11111111000000001111111100000000, 0b11111111111111110000000000000000};
    for(uint32_t sh=0; sh < NSHARES; sh++)
    {
        Bpart[sh] = 0;
        for(uint32_t j=0; j < INP_BITS; j++)
        {
            uint32_t tmp = __builtin_parity(reg[sh] & BOOLEANMASK[j]);
            Bpart[sh] ^= tmp << j;
        }
    }

    for(uint32_t sh=0; sh < NSHARES; sh++)
    {
        B[sh] |= Bpart[sh] << loc;
    }
    PROFILE_STEP_STOP(22);
}

void A2B_Rotate(size_t NCOEFF, size_t modulus, uint32_t B[NCOEFF][NSHARES], const uint32_t A[NCOEFF][NSHARES])
{
    uint32_t Ap[NCOEFF][NSHARES];

    ////////////////////////////////////////////////////////////
    ///                    Step 1 : A2C                      ///
    ////////////////////////////////////////////////////////////
    
    memcpy(Ap, A, NCOEFF * NSHARES * sizeof(uint32_t));

    for(uint32_t k=0; k<NCOEFF; k++)
    {
        A2B_inner(B[k], Ap[k], modulus); //0b1111111111000 = 8184
    }
}


