/*
                                 /\
                                /  \
                               |    |
                             --:'''':--
                               :'_' :
                               _:"":\___
                ' '      ____.' :::     '._
               . *=====<<=)           \    :
                .  '      '-'-'\_      /'._.'
                                 \====:_ ""
                                .'     \\
                               :       :
                              /   :    \
                             :   .      '.
             ,. _        snd :  : :      :
          '-'    ).          :__:-:__.;--'
        (        '  )        '-'   '-'
     ( -   .00.   - _
    (    .'  _ )     )
    '-  ()_.\,\,   -

    UNDER CONSTRUCTION
*/

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <gmp.h>
#include <stdbool.h>
#include <time.h>

#define VERBOSE
// #undef VERBOSE
#define TEST_NUM 10
#define TEST_BITS_64 1

int64_t mdiv(int64_t *x, size_t n, int64_t y);

int main() {
    clock_t start, end;
    mpz_t dummy, number, number2;
    gmp_randstate_t state;
    mpz_init(dummy);
    mpz_init(number);
    mpz_init(number2);
    gmp_randinit_default(state);
    int64_t dzielne[TEST_NUM][TEST_BITS_64];
    int64_t dzielniki[TEST_NUM];
    int64_t ilorazy[TEST_NUM][TEST_BITS_64];
    for (int i = 0; i < TEST_NUM; i++) {
        for (int j = 0; j < TEST_BITS_64; j++) {
            int64_t random_number = 0;
            for (int k = 0; k < 4; k++) {
                dzielne[i][j] |= rand() << (16 * k);
            }
        }
        mpz_set_si(number, -1);
        for(int64_t j = 0; j < TEST_BITS_64; j++)
            dzielne[i][j] = (int64_t) mpz_getlimbn(number,j);
        dzielniki[i] = (rand() << 48) + (rand() << 32) + (rand() << 16) + rand();
        mpz_t dzielnik;
        mpz_init_set_si(dzielnik, dzielniki[i]);
        mpz_tdiv_q(number2, number, dzielnik);
        for(int64_t j = 0; j < TEST_BITS_64; j++)
            ilorazy[i][j] = (int64_t) mpz_getlimbn(number2,j);
        printf("%ld %ld %ld %d \n", dzielne[i][0], dzielniki[i], ilorazy[i][0], i);
    }

    start = clock();

    for(int i = 0; i < TEST_NUM; i++){
        mdiv(dzielne[i], TEST_BITS_64, dzielniki[i]);
    }

    end = clock();

    bool pass = true;
    for(int i = 0; i < TEST_NUM; i++){
        for(int j = 0; j < TEST_BITS_64; j++){
            if(dzielne[i][j] != ilorazy[i][j]){
                pass = false;
                printf("W teście %d w ilorazie pod indeksem %d \n"
                        "jest        0x%016" PRIx64 ",\n"
                        "powinno być 0x%016" PRIx64 ".\n",
                        i, j, dzielne[i][j], ilorazy[i][j]);
            }
        }
        if(!pass)
            return 1;
    }

#ifdef VERBOSE
    printf("Time elapsed %.4f \n", ((double) (end - start)) * 1000 / CLOCKS_PER_SEC);
#endif

    return 0;
}