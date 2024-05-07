#define _GNU_SOURCE

#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <signal.h>
#define SIZE(x) (sizeof x / sizeof x[0])

volatile sig_atomic_t sigfpe_occurred = 0;

jmp_buf jmpbuf;
bool pass = true;

int64_t mdiv(int64_t *x, size_t n, int64_t y);

typedef struct {
  size_t  const  n; // rozmiar dzielnej
  int64_t const *x; // dzielna
  int64_t const  y; // dzielnik
  bool returns_error; // czy zwraca błąd
} test_data_t;

static const test_data_t test_data[] = {
    {1, (int64_t[1]){0xdead1337c0deB1FF}, 0, 1}, // SIGFPE
    {1, (int64_t[1]){0x8000000000000001}, -1, 0},
    {1, (int64_t[1]){0x8000000000000000}, -1, 1}, // SIGFPE

    {2, (int64_t[2]){0,0x8000000000000000}, -1, 1}, // SIGFPE
    {2, (int64_t[2]){1,0x8000000000000000}, -1, 0},
    {2, (int64_t[2]){0x00DEC1A551F1ED00, 0x00DEC1A551F1ED00}, 0, 1}, // SIGFPE
};

void sigfpe_handler() {
    sigfpe_occurred = 1;
    longjmp(jmpbuf, 1);
}

/*
          /`·.¸
         /¸...¸`:·
     ¸.·´  ¸   `·.¸.·´)
    : © ):´;      ¸  {
     `·.¸ `·  ¸.·´\`·¸)
         `\\´´\¸.·´
*/

int main() {
    if (signal(SIGFPE, sigfpe_handler) == SIG_ERR) {
        perror("signal");
        return 1;
    }
    for (size_t test = 0; test < SIZE(test_data); ++test) {
        size_t n = test_data[test].n;
        int64_t *work_space = malloc(n * sizeof (int64_t));
        assert(work_space);
        memcpy(work_space, test_data[test].x, n * sizeof (int64_t));

        struct sigaction act;
        struct sigaction oldact;
        memset(&act, 0, sizeof(act));
        act.sa_handler = sigfpe_handler;
        act.sa_flags = 0x40000000;
        sigaction(SIGFPE, &act, &oldact);

        if (setjmp(jmpbuf) == 0) {
            mdiv(work_space, n, test_data[test].y);
        }
        sigaction(SIGFPE, &oldact, &act);

        if (sigfpe_occurred != test_data[test].returns_error) {
            if(sigfpe_occurred)
                printf("MDIV crashed, but shouldn't.\n");
            else
                printf("MDIV worked, but shouldn't.\n");
            pass = false;
        }
        sigfpe_occurred = 0;
    }
    return !pass;
}