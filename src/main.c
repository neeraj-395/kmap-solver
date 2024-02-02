#include "libutils.h"
#include "tabular_method.h"
#include "branching_method.h"
#define EXTRA 2 
#define _1INX_1DIFF 2
#define INIT_STAGE_VAL 1
#define MAX_VAR 21
#define NO_INPUT_MSG "Error: No input has been received.\n"
#define INVALID_ARG1_MSG "Error: The provided argument is invalid (minterm).\n"
#define INVALID_SYNTAX_MSG "Error: Missing or invalid \"dcare=\" argument.\n"
#define INVALID_ARG3_MSG "Error: The third argument is invalid (don't care).\n"
#define BUFFER_ERR "Error: buffer requested is too small.\n"

int main(int argc, const char **minChar)
{
    if (argc <= 1)
        return (fprintf(stderr, NO_INPUT_MSG) || EXIT_FAILURE);
    else if (!isInputValid(1, minChar[1]))
        return (fprintf(stderr, INVALID_ARG1_MSG) || EXIT_FAILURE);
    else if (argc >= 3 && strcmp(minChar[2], "dcare="))
        return (fprintf(stderr, INVALID_SYNTAX_MSG) || EXIT_FAILURE);
    else if (argc >= 4 && !isInputValid(3, minChar[3]))
        return (fprintf(stderr, INVALID_ARG3_MSG) || EXIT_FAILURE);

    if (!kmap->_1sC)
        return (putchar('0') - '0');

    else
        initialize_minimization_control_set(minChar);

    /*  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
        X   Prime Implicants Identification Implimentation Using Quine McCluskey Algorithm   X
        XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  */

    pic_set *pi = malloc(sizeof(pic_set));

    pi->minBsize = kmap->minSize;
    pi->minBin = (char **)_2d_array(pi->minBsize, kmap->varSize, sizeof(char));
    kmap->piBin = (char **)_2d_array(kmap->minSize * EXTRA, kmap->varSize, sizeof(char));

    decimal_to_binary(pi, kmap->minDec, kmap->varSize);

    size_t currStageofminization = INIT_STAGE_VAL;

    while (1)
    {
        pi->titrack = malloc(sizeof(bool) * pi->minBsize);
        memset(pi->titrack, true, pi->minBsize); // asumming every minterm is essential

        pi->cmpMAX = approxMAXcomparision(kmap->varSize, currStageofminization, pi->minBsize);

        if (pi->cmpMAX)
            pi->cmpSet = (USIGN_INT **)_2d_array(pi->cmpMAX, _1INX_1DIFF, sizeof(USIGN_INT));

        pi->cmpSize = intialize_comparision_set(pi, kmap->varSize);

        get_prime_implicant(pi->minBin, pi->titrack, pi->minBsize);

        if (!pi->cmpSize)
        {
            pi->cmpSet = (USIGN_INT **)_2d_arrfree((void **)pi->cmpSet, pi->cmpMAX);
            free(pi->titrack);
            break; // No comparable pairs is left (break the loop)
        }

        pi->buffSize = pi->cmpSize;

        pi->buffer = (char **)_2d_array(pi->buffSize, kmap->varSize, sizeof(char));

        if (!pi->buffer)
            return (fprintf(stderr, BUFFER_ERR) || EXIT_FAILURE);

        
        minimization_process(pi, kmap->varSize);


    //  XXXXXXXXXXXX free buffer and backup its data in to minBin XXXXXXXXXXXXXXXXXXX
        
        pi->minBin = (char **)_2d_arrfree((void **)pi->minBin, pi->minBsize);

        pi->minBin = (char **)_2d_array(pi->buffSize, kmap->varSize, sizeof(char));

            for (size_t i = 0; i < pi->buffSize; i++)
                    memcpy(pi->minBin[i], pi->buffer[i], kmap->varSize);

        pi->buffer = (char **)_2d_arrfree((void **)pi->buffer, pi->buffSize);

        pi->minBsize = pi->buffSize;
        
    //  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

        pi->cmpSet = (USIGN_INT **)_2d_arrfree((void **)pi->cmpSet, pi->cmpMAX);

        free(pi->titrack);

        currStageofminization++;
    }

    pi->minBin = (char **)_2d_arrfree((void **)pi->minBin, pi->minBsize);
    free(pi); // free the structure memory

    /*  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
        X  Essential Prime Implicants Identification Implimentation Using Dominance Method.  X
        X  For Cyclic Boolean Functions Branching Method is Used.                            X
        XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  */

    pit_set DONT_USE_IT, *epi = &DONT_USE_IT;

    epi->nom = kmap->_1sC;
    epi->nopi = kmap->pi_currinx;

    epi->table = (char **)_2d_array(epi->nom, epi->nopi, sizeof(char));

    draw_pitable(epi, kmap->piBin, kmap->minDec, kmap->varSize);

    free(kmap->minDec); // No use after that

    ctt_set DONT_USE, *track = &DONT_USE;

    track->min = malloc(epi->nom * sizeof(char));
    track->epi = malloc(epi->nopi * sizeof(char));

    memset(track->min, UNCOVERED, epi->nom);
    memset(track->epi, UNCOVERED, epi->nopi);

    while (uncovered_terms(track->min, epi->nom, NULL))
    {
        covered_minterms_eliminator(epi, track);

        non_essential_primes_eliminator(epi, track);

        collect_essential_primes(epi, track);

        if (uncovered_terms(track->min, epi->nom, NULL))
            cyclic_function_method(epi, track);
    }

    for (size_t i = 0; i < epi->nopi; i++)
        if (track->epi[i] == ESSENTIAL)
            printf("%.*s\n", (int)kmap->varSize, kmap->piBin[i]);

    kmap->piBin = (char **)_2d_arrfree((void **)kmap->piBin, kmap->minSize * EXTRA);
    epi->table = (char **)_2d_arrfree((void **)epi->table, epi->nom);
    free(track->epi);
    free(track->min);
    return 0;
}