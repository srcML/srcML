#if sizeof(struct s) > 5
    printf("size of structue larger then 5\n");
#endif

// #if, #else, #endif Examples.
#if DLEVEL > 5
    #define SIGNAL  1
    #if STACKUSE == 1
        #define STACK   200
    #else
        #define STACK   100
    #endif
#else
    #define SIGNAL  0
    #if STACKUSE == 1
        #define STACK   100
    #else
        #define STACK   50
    #endif
#endif

// #elif example
#if DLEVEL == 0
    #define STACK 0
#elif DLEVEL == 1
    #define STACK 100
#elif DLEVEL > 5
    display( debugptr );
#else
    #define STACK 200
#endif

// #ifdef example
#ifdef NDEBUG
    if(x >20) {
        abort();
    }
#endif

// #ifndef example
#ifndef test
#define final
#endif