#ifndef TABULAR_METHOD_H
#define TABULAR_METHOD_H
#ifdef _WIN32
#define STRTOK strtok_s
#else
#define STRTOK strtok_r
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>
#define MAX(X, Y) (X >= Y) ? X : Y

typedef struct minimization_control_set
{
    char **piBin;
    size_t *minDec;
    unsigned short int _1sC;
    unsigned short int dontC;
    unsigned short int minSize;
    unsigned short int varSize;
    unsigned short int pi_fillInx;
} MCS;

MCS NOT_FOR_USE = {NULL, NULL, 0, 0, 0, 0, 0}, *Kmap = &NOT_FOR_USE;

typedef struct prime_implicant_identification_control_set
{
    size_t cmpMAX;
    bool *titrack;
    char **minBin;
    char **buffer;
    unsigned short int **cmpSet;
    unsigned short int cmpSize;
    unsigned short int minBsize;
    unsigned short int buffSize;
    unsigned short int diff_bitinx;
} PIICS;
/*
void decimal_to_binary(PIICS *pi);
void get_prime_implicant(PIICS *pi);
void backup_and_free_buffer(PIICS *pi);
bool isMintermExist(PIICS *pi, size_t M);
bool isInputValid(int inx, const char *input);
void **_2d_arrfree(void **array, size_t rowlength);
size_t MAXcomparision(size_t var, size_t currstage);
size_t strtok_to_atoi(const char *str, size_t *inx);
size_t bit_diff_count(PIICS *pi, size_t M, size_t N);
unsigned short int intialize_comparision_set(PIICS *pi);
void initialize_minimization_control_set(const char **minChar);
void **_2d_array(size_t rowlength, size_t colength, size_t size);*/

void decimal_to_binary(PIICS *pi)
{
    for (size_t i = 0, w = 0; i < pi->minBsize; i++)
    {
        size_t decVal = Kmap->minDec[i];
        for (int j = (Kmap->varSize - 1); j >= 0; j--)
        {
            pi->minBin[w][j] = ('0' + (decVal % 2));
            decVal >>= 1;
        }
        w++;
    }
}

bool isMintermExist(PIICS *pi, size_t M)
{
    for (int i = 0; i < Kmap->varSize; i++)
        if (pi->minBin[M][i] == '_' && pi->diff_bitinx > i)
            return true;
    return false;
}

size_t bit_diff_count(PIICS *pi, size_t M, size_t N)
{
    size_t bit_diffC = 0;
    for (int i = 0; i < Kmap->varSize; i++)
    {
        if (pi->minBin[M][i] != pi->minBin[N][i])
        {
            bit_diffC++;
            pi->diff_bitinx = (unsigned short int)i;
        }
    }
    return bit_diffC;
}

unsigned short int intialize_comparision_set(PIICS *pi)
{
    unsigned short int inx = 0;
    for (size_t M = 0; M < (size_t)pi->minBsize; M++)
    {
        for (size_t N = (M + 1); N < (size_t)pi->minBsize; N++)
        {
            if (bit_diff_count(pi, M, N) == 1)
            {
                if (!isMintermExist(pi, M))
                {
                    pi->cmpSet[inx][0] = M;
                    pi->cmpSet[inx][1] = pi->diff_bitinx;
                    inx++;
                }
                pi->titrack[M] = false;
                pi->titrack[N] = false;
            }
        }
    }
    return inx;
}

void get_prime_implicant(PIICS *pi)
{
    for (size_t i = 0; i < pi->minBsize; i++)
    {
        if (pi->titrack[i])
        {
            memcpy(Kmap->piBin[Kmap->pi_fillInx], pi->minBin[i], Kmap->varSize);
            Kmap->pi_fillInx++;
        }
    }
}

void **_2d_array(size_t rowlength, size_t colength, size_t size)
{
    if (rowlength < 1 || colength < 1)
        return NULL;
    void **arr = malloc(rowlength * sizeof(void *));

    if (!arr)
    {
        perror("Memory allocation failed at fn(_2d_array)(row)");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < rowlength; i++)
    {
        arr[i] = malloc(colength * size);
        if (!arr[i])
        {
            for (size_t j = 0; j < i; j++)
                free(arr[j]);

            free(arr);
            perror("Memory allocation failed at fn(_2d_array)(col)");
            exit(EXIT_FAILURE);
        }
    }

    return arr;
}

void **_2d_arrfree(void **array, size_t rowlength)
{
    if (!array || !rowlength)
        return array;
    for (size_t i = 0; i < rowlength; i++)
        free(array[i]);
    free(array);
    return NULL;
}

size_t MAXcomparision(size_t var, size_t currstage)
{
    if (var < currstage)
        return 0;
    size_t fac_var = (size_t)tgammaf(var + 1);                             
    size_t fac_Itera = (size_t)tgammaf(currstage + 1);             
    size_t fac_subvI = (size_t)tgammaf(var - currstage + 1); 
    size_t _2expo_subvI = (size_t)powf(2, var - currstage);
    return (fac_var * _2expo_subvI) / (fac_Itera * fac_subvI);
}

void strtok_to_atoi(const char *str, size_t *inx, size_t *max_val) //improvement required
{
    char *context = NULL;
    char *tmp = strdup(str);
    char *token = STRTOK(tmp, ",", &context);

    if (!tmp || !token)
    {
        perror("Memory allocation failed at fn(strtok_s_to_atoi)");
        exit(EXIT_FAILURE);
    }

    while (token)
    {
        Kmap->minDec[*inx] = (size_t)atoi(token);
        *max_val = MAX(*max_val, Kmap->minDec[*inx]);
        token = STRTOK(NULL, ",", &context);
        (*inx)++;
    }

    free(tmp);
}

void initialize_minimization_control_set(const char **minChar) //improvement required
{
    Kmap->minDec = malloc(sizeof(size_t) * Kmap->minSize);

    if (!Kmap->minDec)
    {
        perror("Memory allocation failed at fn(initMCS)");
        exit(EXIT_FAILURE);
    }

    size_t decInx = 0, max_val = 0;

    if(Kmap->_1sC) strtok_to_atoi(minChar[1], &decInx, &max_val);

    if(Kmap->dontC) strtok_to_atoi(minChar[3], &decInx, &max_val);

    Kmap->varSize = (max_val) ? (size_t)ceilf(log2f((float)max_val + 1)) : 1;
}

bool isInputValid(int inx, const char *input)
{
    if (!strcmp(input, "(null)"))
        return true;
    bool numFlag = false;
    while (*input)
    {
        if (isdigit(*input))
        {
            if (!numFlag)
            {
                Kmap->_1sC += (inx == 1) ? 1 : 0;
                Kmap->dontC += (inx == 3) ? 1 : 0;
                numFlag = true;
            }
        }
        else if (*input != ',')
        {
            return false; // Invalid character found
        }
        else if (*input == ',')
        {
            numFlag = false;
        }
        input++;
    }
    return true; // All characters are valid
}

void backup_and_free_buffer(PIICS *pi)
{
    if (!pi->minBsize || !pi->buffSize)
        return;
    pi->minBin = (char **)_2d_arrfree((void **)pi->minBin, pi->minBsize);

    pi->minBin = (char **)_2d_array(pi->buffSize, Kmap->varSize, sizeof(char));

    for (size_t i = 0; i < pi->buffSize; i++)
        memcpy(pi->minBin[i], pi->buffer[i], sizeof(char) * Kmap->varSize);

    pi->buffer = (char **)_2d_arrfree((void **)pi->buffer, pi->buffSize);

    pi->minBsize = pi->buffSize;
}

#endif