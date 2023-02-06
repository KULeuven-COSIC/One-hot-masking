#ifndef RANDOMBYTES_H
#define RANDOMBYTES_H

#include <stdint.h>
#include <stddef.h>

#ifdef DEBUG
uint32_t rng_get_random_blocking(void);
#else
#include <libopencm3/stm32/rng.h>
#endif

// fully deterministic randomness
int randombytes(uint8_t *obuf, size_t len);

// trng
#if defined(PROFILE_STEP_RAND) || defined(PROFILE_TOP_RAND)
    #undef NORANDOMSAMPLING
    extern uint64_t nb_randombytes;
    uint32_t rng_count_get_random_blocking(void);
    #define random_uint32() (rng_count_get_random_blocking())                 
    #define random_uint64() (((uint64_t)rng_count_get_random_blocking()) | ((uint64_t)rng_count_get_random_blocking()) << 32)
    #define random_uint32_in() (random_uint32())
    #define random_uint64_in() ((uint64_t)random_uint64())
#else
    #if defined(NORANDOMSAMPLING)
        extern uint32_t U[NORANDOMSAMPLING];
        extern uint32_t rng_idx;
        uint32_t random_uint32_U(void);
        void random_init_U(void);

        #define random_uint32_in() (random_uint32_U())
        #define random_uint64_in() (((uint64_t)random_uint32_U()) | ((uint64_t)random_uint32_U()) << 32)
    #else
        #define random_uint32_in() (random_uint32())
        #define random_uint64_in() ((uint64_t)random_uint64())
    #endif

    #define random_uint32() (rng_get_random_blocking())
    #define random_uint64() (((uint64_t)rng_get_random_blocking()) | ((uint64_t)rng_get_random_blocking()) << 32)
#endif

#endif /* RANDOMBYTES_H */