#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int64_t mdiv(int64_t *x, size_t n, int64_t y);

int main(){
    int64_t x[] = {0x3642b04e230d8c52, 0x472339fd79eb3b31};
    int64_t r = mdiv(x, 2, 0x53c11f6ae8db708a);
    printf(" 0x%016" PRIx64 "  0x%016" PRIx64 " %ld \n", x[0], x[1], r);
    return 0;
}