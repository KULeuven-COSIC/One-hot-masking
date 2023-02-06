#include "randombytes.h"
#include "hal.h"
#include "A2B.h"
#include "A2B_rotate.h"
#include "A2B_table.h"
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <assert.h>

static void get_rand(size_t ncoeffs, uint32_t mod, uint32_t x[ncoeffs])
{
    for (size_t j = 0; j < ncoeffs; j++)
    {
        x[j] = random_uint32() & mod;
    }
}

static void mask(size_t nshares, size_t ncoeffs, size_t modulusmask, uint32_t x_masked[ncoeffs][nshares], uint32_t x[ncoeffs])
{
    for (size_t i = 0; i < ncoeffs; i++)
    {
        x_masked[i][0] = x[i];

        for (size_t j = 1; j < nshares; j++)
        {
            uint32_t R = random_uint32() & modulusmask;
            x_masked[i][0] = (x_masked[i][0] - R) & modulusmask;
            x_masked[i][j] = R;
        }
    }
}

static int test_A2B(void)
{
    uint32_t result;
    uint32_t flag;
    uint32_t public_A[32];
    uint32_t A[32][NSHARES], B[32][NSHARES];
    uint32_t MODULUSMASK = MASK(BITS);

    PROFILE_TOP_INIT();

    hal_send_str("=====Testing A2B====");

    for (size_t i = 0; i < NTESTS; i++)
    {
        get_rand(32, MODULUSMASK, public_A);

        // create reencrypted poly as a (correct) sharing of public poly
        mask(NSHARES, 32, MODULUSMASK, A, public_A);

        flag = 0;
        for(uint32_t k=0; k<32; k++)
        {
            result = 0;
            for (size_t j = 0; j < NSHARES; j++)
            {
                result += A[k][j];
            }
            result = result & MODULUSMASK;
            if (result != public_A[k])
            {
                flag = 1;
            }
        }

        if (flag != 0)
        {
            hal_send_str("[FAIL] sharing");
        }
        else{
            hal_send_str("correct sharing");
        }

        hal_send_str("===== Start ====");

        #if defined(NORANDOMSAMPLING)
            rng_idx = 0;
            random_init_U();
        #endif
        PROFILE_TOP_START();
        #ifdef CLASSICALNB
        A2B_NB(32, BITS, B, A);
        #endif
        #ifdef CLASSICAL
        A2B_bitsliced(NSHARES, BITS, B, A);
        #endif
        #ifdef ROTATE
        A2B_Rotate(32, BITS, B, A);
        #endif
        #ifdef TABLE
        A2B_table(32, BITS, B, A);
        #endif
        PROFILE_TOP_STOP();
        hal_send_str("===== End ====");
        
        flag = 0;
        for(uint32_t k=0; k<32; k++)
        {
            result = 0;
            for (size_t j = 0; j < NSHARES; j++)
            {
                result ^= B[k][j];
            }
            if (result != public_A[k])
            {
                flag = 1;
            }
        }

        assert(flag==0);

        if (flag != 0)
        {
            hal_send_str("[FAIL] result");
        }
        else{
            hal_send_str("correct");
        }
    }
    
    return 0;
}

int main(void)
{
    hal_setup();
    test_A2B();
    return 0;
}