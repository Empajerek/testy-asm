#include <iostream>
#include <cinttypes>
#include <ctime>
#include <random>
#include <boost/multiprecision/cpp_int.hpp>
#include <iomanip>
#include <vector>
#include <iterator>

using namespace boost::multiprecision;

#define VERBOSE
// #undef VERBOSE
#define TEST_NUM 1000
#define TEST_BITS_64 500

extern "C" int64_t mdiv(int64_t *x, size_t n, int64_t y);

int main() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<int64_t> dis(INT64_MIN, INT64_MAX);
    clock_t start, end;
    int64_t dzielne[TEST_NUM][TEST_BITS_64];
    int64_t dzielniki[TEST_NUM];
    int64_t ilorazy[TEST_NUM][TEST_BITS_64];

    for(int i = 0; i < TEST_NUM; i++){
        dzielniki[i] = dis(gen);
        for(int j = 0; j < TEST_BITS_64; j++){
            dzielne[i][j] = dis(gen);
        }

        cpp_int x = dzielne[i][TEST_BITS_64-1];

        for(int j = TEST_BITS_64 - 2; j >= 0; j--){
            x <<= 64;
            x += (uint64_t) dzielne[i][j];
        }

        x /= dzielniki[i];

        std::vector<int64_t> v(TEST_BITS_64);
        export_bits(x, std::back_inserter(v), 64);

        for(int j = 0; j < TEST_BITS_64; j++){
            ilorazy[i][j] = v.back();
            v.pop_back();
        }

        if(x < 0){
            int carry = 1;
            for(int j = 0; j < TEST_BITS_64; j++){
                ilorazy[i][j] = (~ ilorazy[i][j]) + carry;
                if(ilorazy[i][j] != 0){
                    carry = 0;
                }
            }
        }

        // printf("%d :  0x%016" PRIx64 " 0x%016" PRIx64 " / 0x%016" PRIx64 "  = 0x%016" PRIx64 " 0x%016" PRIx64 "  \n",
        //     i, dzielne[i][0], dzielne[i][1], dzielniki[i], ilorazy[i][0], ilorazy[i][1]);
    }

    start = clock();

    for(int i = 0; i < TEST_NUM; i++){
        //printf("%ld / %ld = ", dzielne[i][0], dzielniki[i]);
        mdiv(dzielne[i], TEST_BITS_64, dzielniki[i]);
        // printf("%ld r %ld \n", dzielne[i][0], reszta);
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
    printf("Time elapsed %.4f \n ms", ((double) (end - start)) * 1000 / CLOCKS_PER_SEC);
#endif

    return 0;
}