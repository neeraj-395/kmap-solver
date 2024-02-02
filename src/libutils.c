#include "libutils.h"
#include "hashtable.h"
#define THROW_ERROR(X) {fprintf(stderr, X); exit(1);}
#define FREE_ALLOCATED_MEMORY  free(tmp); free(kmap->minDec); destroy_hash();
#define ERR_DUPLICATE_MIN "Error: Duplicate values are not allowed.\n"

static MCS NOT_FOR_USE = {NULL, NULL, 0, 0, 0, 0, 0};
MCS *kmap = &NOT_FOR_USE;

bool isInputValid(size_t inx, const char *input)
{
    if (!strcmp(input, "(null)"))
        return true;
    bool numFlag = false;
    while (*input)
    {
        if (isdigit(*input)) {
            if(!numFlag){ 
            kmap->_1sC += (inx == 1) ? 1 : 0;
            kmap->dontC += (inx == 3) ? 1 : 0;
            numFlag = true;
            }
        }

        else if (*input != ',')
            return false; // Invalid character found

        else if (*input == ',')
            numFlag = false;
        
        input++;
    }
    return true; // All characters are valid
}

void **_2d_array(size_t row, size_t col, size_t size)
{
    if (row < 1 || col < 1)
        return NULL;
    void **arr = malloc(row * sizeof(void *));

    if (!arr)
        THROW_ERROR("Memory allocation failed at fn(_2d_array)(row)")

    for (size_t i = 0; i < row; i++)
    {
        arr[i] = malloc(col * size);
        if (!arr[i])
        {
            for (size_t j = 0; j < i; j++)
                free(arr[j]);

            free(arr);
            THROW_ERROR("Memory allocation failed at fn(_2d_array)(col)")
        }
    }

    return arr;
}

void **_2d_arrfree(void **array, size_t row)
{
    if (!array || !row)
        return array;
    for (size_t i = 0; i < row; i++)
        free(array[i]);
    free(array);
    return NULL;
}

static void strtok_to_atoi(const char *str, ULL_INT *max_val) //improvement required
{
    static size_t __inx__ = 0;
    char *context = NULL;
    char *tmp = strdup(str);
    char *token = STRTOK(tmp, ",", &context);

    if (!tmp || !token)
        THROW_ERROR("Memory allocation failed at fn(strtok_s_to_atoi)")

    while (token)
    {
        ULL_INT min_in_dec = (ULL_INT)ATOI(token);
        
        if(search_in_hash(min_in_dec)){
            FREE_ALLOCATED_MEMORY
            THROW_ERROR(ERR_DUPLICATE_MIN)
        } else {
            kmap->minDec[__inx__] = min_in_dec;
            (*max_val) = MAX((*max_val), min_in_dec);
            token = STRTOK(NULL, ",", &context);
            insert_in_hash(min_in_dec);
            __inx__++;
        }
    }

    free(tmp);
}

void initialize_minimization_control_set(const char **minChar) //improvement required
{
    kmap->minSize = kmap->_1sC + kmap->dontC;

    kmap->minDec = malloc(sizeof(size_t) * kmap->minSize);

    if (!kmap->minDec)
        THROW_ERROR ("Memory allocation failed at fn(initMCS)")

    create_hash(kmap->minSize);

    ULL_INT max_val = 0;

    if(kmap->_1sC) strtok_to_atoi(minChar[1], &max_val);

    if(kmap->dontC) strtok_to_atoi(minChar[3], &max_val);
    
    destroy_hash();

    if(!max_val) { putchar('0'); exit(EXIT_SUCCESS);}

    while(max_val > 0){ max_val >>= 1; kmap->varSize++; }
}

void get_prime_implicant(char **minBin, bool *titrack, size_t minBsize)
{
    for (size_t i = 0; i < minBsize; i++)
    {
        if (titrack[i])
        {
            memcpy(kmap->piBin[kmap->pi_currinx], minBin[i], kmap->varSize);
            kmap->pi_currinx++;
        }
    }
}