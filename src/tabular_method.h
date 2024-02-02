#ifndef TABULAR_METHOD_H
#define TABULAR_METHOD_H
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#define MAX_VAR 21
#define USIGN_INT unsigned int
#define ULL_INT unsigned long long int

typedef struct prime_identification_control_set
{
    size_t cmpMAX;
    bool *titrack;
    char **minBin;
    char **buffer;
    size_t cmpSize;
    size_t minBsize;
    size_t buffSize;
    size_t diff_bitinx;
    USIGN_INT **cmpSet;    
} pic_set;

size_t approxMAXcomparision(size_t var, size_t currstage, size_t minBSize);
size_t intialize_comparision_set(pic_set *pi, size_t var);
void decimal_to_binary(pic_set *pi, ULL_INT *minDec, size_t var);
void minimization_process(pic_set *pi, size_t var);

#endif