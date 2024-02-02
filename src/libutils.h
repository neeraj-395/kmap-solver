#ifndef UTILS_H
#define UTILS_H
#ifdef _WIN32
#define STRTOK strtok_s
#define ATOI _atoi64
#else
#define STRTOK strtok_r
#define ATOI atoll
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#define MAX(X, Y) (X >= Y) ? X : Y
#define ULL_INT unsigned long long int

typedef struct minimization_control_set
{
    char **piBin;
    ULL_INT *minDec; // dont change their order
    size_t _1sC;
    size_t dontC;
    size_t minSize;
    size_t varSize;
    size_t pi_currinx;
} MCS;

extern MCS *kmap;

void **_2d_arrfree(void **array, size_t row);
bool isInputValid(size_t inx, const char *input);
void **_2d_array(size_t row, size_t col, size_t size);
void initialize_minimization_control_set(const char **minChar);
void get_prime_implicant(char **minBin, bool *titrack, size_t minBsize);

#endif
