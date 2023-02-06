#ifndef HAL_H
#define HAL_H

#include <stdint.h>
#include <stdio.h>


#define PROFILE_STEP_INIT() do{}while(0)
#define PROFILE_STEP_START() do{}while(0)
#define PROFILE_STEP_STOP(step) do{}while(0)
#define PROFILE_TOP_INIT() do{}while(0)
#define PROFILE_TOP_START() do{}while(0)
#define PROFILE_TOP_STOP() do{}while(0)

#ifdef DEBUG
    // x86 host : dummy functions
    #define hal_setup() do{}while(0)
    #define hal_send_str(x) printf(x); printf("\n")
    #define hal_get_time() 0
    #define printcycles(a, b) do{(void)(b);}while(0)
#else
    // ARM
    void hal_setup(void);
    void hal_send_str(const char* in);
    uint64_t hal_get_time(void);
    void printcycles(const char *s, uint64_t c);

    #if defined(PROFILE_STEP_CYCLES)
        #undef PROFILE_STEP_INIT
        #undef PROFILE_STEP_START
        #undef PROFILE_STEP_STOP
        #define PROFILE_STEP_INIT() uint64_t t0, t1
        #define PROFILE_STEP_START() t0 = hal_get_time()
        #define PROFILE_STEP_STOP(step) t1 = hal_get_time(); \
                printcycles("Step " #step " cycles:", t1 - t0)
    #elif defined(PROFILE_STEP_RAND)
        #undef PROFILE_STEP_INIT 
        #undef PROFILE_STEP_START
        #undef PROFILE_STEP_STOP
        #define PROFILE_STEP_INIT() do{}while(0)
        #define PROFILE_STEP_START() nb_randombytes = 0
        #define PROFILE_STEP_STOP(step) printcycles("Step " #step " randombytes:", nb_randombytes)
    #else
        #undef PROFILE_TOP_INIT 
        #undef PROFILE_TOP_START 
        #undef PROFILE_TOP_STOP
        #if defined(PROFILE_TOP_RAND)
            #define PROFILE_TOP_INIT() do{}while(0)
            #define PROFILE_TOP_START() nb_randombytes = 0
            #define PROFILE_TOP_STOP() printcycles("A2B randombytes:", nb_randombytes)
        #else // PROFILE_TOP_CYCLES
            #define PROFILE_TOP_INIT() uint64_t t0, t1
            #define PROFILE_TOP_START() t0 = hal_get_time()
            #define PROFILE_TOP_STOP() t1 = hal_get_time(); \
                    printcycles("A2B cycles:", t1 - t0)
        #endif
    #endif
#endif // DEBUG



#endif

