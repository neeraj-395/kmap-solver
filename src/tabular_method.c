#include "tabular_method.h"

static unsigned long long C[MAX_VAR + 1][MAX_VAR + 1];

static unsigned long long int nCr(size_t n, size_t r) {
    static size_t prev_r;
    if (r == 0 || r == n) return 1;
    else if (n < r) return 0;
    else if(n > MAX_VAR || r > MAX_VAR) return 0;
    else if (C[n][r]) return C[n][r];
    for (size_t i = prev_r; i <= n; i++) {
        for (size_t j = prev_r; j <= i && j <= r; j++) {
            if (j == 0 || j == i)
                C[i][j] = 1;
            else
                C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
        }
    }
    prev_r = r;
    return C[n][r];
}

void decimal_to_binary(pic_set *pi, ULL_INT *minDec, size_t var)
{
    for (size_t i = 0, w = 0; i < pi->minBsize; i++)
    {
        ULL_INT decVal = minDec[i];
        for (int j = (var - 1); j >= 0; j--)
        {
            pi->minBin[w][j] = ('0' + (decVal % 2));
            decVal >>= 1;
        }
        w++;
    }
}

static bool isCombinationExist(pic_set *pi, size_t M, size_t var)
{
    for (size_t i = 0; i < var; i++)
        if (pi->minBin[M][i] == '_' && pi->diff_bitinx > i)
            return true;
    return false;
}

static size_t bit_diff_count(pic_set *pi, size_t M, size_t N, size_t var)
{
    size_t bit_diffC = 0;
    for (size_t i = 0; i < var; i++)
    {
        if (pi->minBin[M][i] != pi->minBin[N][i])
        {
            bit_diffC++;
            pi->diff_bitinx = i;
        }
    }
    return bit_diffC;
}

size_t intialize_comparision_set(pic_set *pi, size_t var)
{
    size_t inx = 0;
    for (size_t M = 0; M < (size_t)pi->minBsize; M++)
    {
        for (size_t N = (M + 1); N < (size_t)pi->minBsize; N++)
        {
            if (bit_diff_count(pi, M, N, var) == 1)
            {
                if (!isCombinationExist(pi, M, var))
                {
                    pi->cmpSet[inx][0] = (USIGN_INT)M;
                    pi->cmpSet[inx][1] = (USIGN_INT)pi->diff_bitinx;
                    inx++;
                }
                pi->titrack[M] = false;
                pi->titrack[N] = false;
            }
        }
    }
    return inx;
}

size_t approxMAXcomparision(size_t var, size_t currstage, size_t minBSize)
{
    ULL_INT numCombination = nCr(var, currstage);
    size_t uniqPairIterationCount = (minBSize * (minBSize - 1)) / 2; 
    ULL_INT numPossibleArrangOf_0_1 = (ULL_INT)powl(2.0, (long double)(var - currstage));
    size_t cmpMAX = (size_t)(numCombination * numPossibleArrangOf_0_1);
    return (cmpMAX <= uniqPairIterationCount) ? cmpMAX : uniqPairIterationCount;
}

void minimization_process(pic_set *pi, size_t var) 
{
    for (size_t i = 0; i < pi->buffSize; i++)
    {
        USIGN_INT pair1 = pi->cmpSet[i][0];
        USIGN_INT diff_bit_inx = pi->cmpSet[i][1];

        memcpy(pi->buffer[i], pi->minBin[pair1], sizeof(char) * var);
        pi->buffer[i][diff_bit_inx] = '_';
    }
}