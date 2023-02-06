#include "params.h"
#include "A2C.h"
#include <stdbool.h>
#include "hal.h"
#include "randombytes.h"

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

void arithprintshared(uint32_t *v, uint32_t q)
{
    uint32_t v2 = 0;
    for(uint32_t sh=0; sh < NSHARES; sh++)
    {
        v2 += v[sh];
    }
    v2 = v2 % q;
    uint32_t mask=1<<((sizeof(uint32_t)<<3)-1);
    while(mask) {
        printf("%d", (v2&mask ? 1 : 0));
        mask >>= 1;
    }
    printf(" = %d", v2);
    printf("\n");
}

uint32_t arithshared(uint32_t *v, uint32_t q)
{
    uint32_t v2 = 0;
    for(uint32_t sh=0; sh < NSHARES; sh++)
    {
        v2 += v[sh];
    }
    v2 = v2 % q;
    return v2;
}
#endif

inline static uint32_t ROT(uint32_t R, uint32_t shift)
{
    R = ( (R << shift) | (R >> (REG_BITS - shift)) );
    return R;
}

static void ROT_remask(uint32_t out[NSHARES], uint32_t in[NSHARES])
{
    for(uint32_t sh_in=0; sh_in<NSHARES; sh_in++)
    {
        // rotate + reshare
        out[NSHARES-1] =  ROT(out[NSHARES-1], in[sh_in]); // start with last share; so you can already do reshare in loop
        for(uint32_t sh_out=0; sh_out < NSHARES-1; sh_out++)
        {
            out[sh_out] =  ROT(out[sh_out], in[sh_in]); 

            // if you want to work with new randomness every iteration
            uint32_t r = random_uint32_in();
            out[sh_out] = out[sh_out] ^ r;
            out[NSHARES-1] = out[NSHARES-1] ^ r;

            // // reuse randomness NOT SECURE
            // out[sh_out] = out[sh_out] ^ U[sh_in][sh_out];
            // out[NSHARES-1] = out[NSHARES-1] ^ U[sh_in][sh_out];
        }
    }
}

static void filter(uint32_t out[NSHARES], uint32_t form, uint32_t INP_BITS)
{
    // precalculate filter and shift for below
    uint32_t filter = 1;
    for(uint32_t i= (1 << INP_BITS); i < REG_BITS; i=i<<1)
    {
        filter += filter << i;
    }
    uint32_t filter2 = ((1 << (1<<CARRY_BITS)) - 1);
    uint32_t shift = (1<<INP_BITS) - 1; 

    for(uint32_t sh=0; sh < NSHARES; sh++)
    {
        // take care of positions that are allowed to be non-zero
        for(uint32_t i=0; i < INP_BITS; i++)
        {
            if( ( (form >> i) & 1)==0 )
            {
                out[sh] ^=  out[sh] >> (1 << i);
            }
        }
        out[sh] = out[sh] & filter;

        // take care of carries
        for(uint32_t i=shift; i < REG_BITS; i=i<<1)
        {
            out[sh] ^= out[sh] >> i;
        }
        out[sh] = out[sh] & filter2;
    }
}

void A2C(uint32_t out[NSHARES], uint32_t in[NSHARES], uint32_t template)
{
    uint32_t INP_BITS;

    PROFILE_STEP_INIT();

    while(true)
    {
        if(template >= REG_BITS)
        {
            PROFILE_STEP_START();
            uint32_t in2[NSHARES];
            uint32_t form2;

            INP_BITS = REG_BITS_L - CARRY_BITS;
            for(uint32_t sh=0; sh < NSHARES; sh++)
            {
                in2[sh] = in[sh] & MASK(INP_BITS);
                in[sh] = in[sh] >> INP_BITS;
            }
            form2 = template & MASK(INP_BITS);
            template = template >> INP_BITS;
            PROFILE_STEP_STOP(10);

            PROFILE_STEP_START();
            ROT_remask(out, in2);
            PROFILE_STEP_STOP(11);

            PROFILE_STEP_START();
            filter(out, form2, INP_BITS);
            PROFILE_STEP_STOP(12);
        }
        else
        {
            PROFILE_STEP_START();
            ROT_remask(out, in);
            PROFILE_STEP_STOP(21);

            PROFILE_STEP_START();
            filter(out, template, REG_BITS_L);
            PROFILE_STEP_STOP(22);

            for(uint32_t sh=0; sh < NSHARES; sh++)
            {
                out[sh] = out[sh] & 1;
            }
            break;
        }
    }
}

