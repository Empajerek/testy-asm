#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <stdint.h>
#include <stddef.h>

int64_t mdiv(long long *x, size_t n, int64_t y);

// Function to convert mpz_t to an array of long long integers
long long *mpz_to_longlong_array(mpz_t num, size_t *length) {
    // Get the number of limbs (words) in the mpz_t variable
    size_t limbs = mpz_size(num);

    // Allocate memory for the array of long long integers
    long long *array = (long long *)malloc(limbs * sizeof(long long));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Extract the limbs and store them in the array
    for (size_t i = 0; i < limbs; i++) {
        array[i] = mpz_getlimbn(num, i);
    }

    // Set the length of the array
    *length = limbs;

    return array;
}

int main() {
    mpz_t large_number;
    gmp_randstate_t state;
    mpz_init(large_number);
    gmp_randinit_default(state);
    mpz_urandomb(large_number, state, 512);
    long long divisor = 1234;

    size_t length;
    long long *array = mpz_to_longlong_array(large_number, &length);

    mpz_t quotient;
    mpz_init(quotient);
    mpz_tdiv_q_ui(quotient, large_number, divisor);

    mdiv(array, length, divisor);

    long long *array2 = mpz_to_longlong_array(quotient, &length);

    // Print the array elements
    printf("Array of long long integers:\n");
    for (size_t i = 0; i < length; i++) {
        printf("%lld ", array[i] - array2[i]);
    }
    printf("\n");

    // Free memory
    free(array);
    free(array2);
    mpz_clear(quotient);
    mpz_clear(large_number);

    return 0;
}