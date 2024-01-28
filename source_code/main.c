#include "tabular_method.h"
#include "branching_method.h"
#include "troubleshoot.h"
#define _1INX_1DIFF 2
#define INIT_STAGE_VAL 1
#define USHRT_INT unsigned short int
#define NO_INPUT_MSG "Error: No input has been received.\n"
#define INVALID_ARG1_MSG "Error: The provided argument is invalid (minterm).\n"
#define INVALID_SYNTAX_MSG "Error: Missing or invalid \"!care=\" argument.\n"
#define INVALID_ARG3_MSG "Error: The third argument is invalid (don't care).\n"
#define BUFFER_ERR "Error: buffer requested size: %d is too small.\n"


int main(int argc, const char **minChar){

    if (argc <= 1) 
        return (fprintf(stderr, NO_INPUT_MSG) || EXIT_FAILURE);
    else if(!isInputValid(1,minChar[1])) 
        return (fprintf(stderr, INVALID_ARG1_MSG) || EXIT_FAILURE);
    else if(argc >= 3 && strcmp(minChar[2],"!care="))
        return (fprintf(stderr, INVALID_SYNTAX_MSG) || EXIT_FAILURE);
    else if(argc >= 4 && !isInputValid(3,minChar[3])) 
        return (fprintf(stderr, INVALID_ARG3_MSG) || EXIT_FAILURE);
  
    Kmap->minSize = Kmap->_1sC + Kmap->dontC;    
    
    if (!Kmap->minSize || !Kmap->_1sC) return (putchar('0') - '0');

    else initialize_minimization_control_set(minChar);

/*  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    X   Prime Implicants Identification Implimentation Using Quine McCluskey Algorithm   X
    XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  */ 

    PIICS *pi = malloc(sizeof(PIICS));

    pi->minBsize = Kmap->minSize;
    pi->minBin = (char **)_2d_array(pi->minBsize, Kmap->varSize, sizeof(char));
    Kmap->piBin = (char **)_2d_array(Kmap->minSize * 2, Kmap->varSize, sizeof(char));

    decimal_to_binary(pi);

    size_t currStageofminization = INIT_STAGE_VAL;

    while(1)
    {
        pi->titrack = malloc(sizeof(bool) * pi->minBsize);
        memset(pi->titrack,true, pi->minBsize); //asumming every minterm is essential

        pi->cmpMAX = MAXcomparision(Kmap->varSize,currStageofminization);
        
        if(pi->cmpMAX)
            pi->cmpSet = (USHRT_INT **)_2d_array(pi->cmpMAX, _1INX_1DIFF, sizeof(USHRT_INT));

        pi->cmpSize = intialize_comparision_set(pi);

        get_prime_implicant(pi);

        if(!pi->cmpSize) 
        { 
            pi->cmpSet = (USHRT_INT **)_2d_arrfree((void **)pi->cmpSet, pi->cmpMAX);
            free(pi->titrack); 
            break; // No comparable pairs is left (break the loop)
        } 

        pi->buffSize = pi->cmpSize;

        pi->buffer = (char **)_2d_array(pi->buffSize, Kmap->varSize, sizeof(char));

        if(!pi->buffer) 
            return fprintf(stderr, BUFFER_ERR, pi->buffSize) || EXIT_FAILURE;

        // comparision sub-stage
        {
            for (size_t i = 0; i < pi->buffSize; i++)
            {
                USHRT_INT pair1 = pi->cmpSet[i][0];
                USHRT_INT diffbinx = pi->cmpSet[i][1]; 

                memcpy(pi->buffer[i], pi->minBin[pair1], sizeof(char) * Kmap->varSize);
                pi->buffer[i][diffbinx] = '_';
            }
        }

        backup_and_free_buffer(pi);

        pi->cmpSet = (USHRT_INT **)_2d_arrfree((void **)pi->cmpSet,pi->cmpMAX);
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

    epi->nom = (size_t)Kmap-> _1sC;
    epi->nopi = (size_t)Kmap->pi_fillInx;
    
    epi->table = (char **)_2d_array(epi->nom, epi->nopi, sizeof(char));

    draw_pitable(epi);

    ctt_set DONT_USE, *track = &DONT_USE;

    track->min = malloc(epi->nom * sizeof(char));
    track->epi = malloc(epi->nopi * sizeof(char));

    memset(track->min, UNCOVERED, epi->nom);
    memset(track->epi, UNCOVERED, epi->nopi);

    while (uncovered_terms(track->min, epi->nom, NULL))
    {
        covered_minterms_eliminator(epi,track);

        non_essential_primes_eliminator(epi,track);

        collect_essential_primes(epi,track);

        if (uncovered_terms(track->min, epi->nom, NULL))
            cyclic_function_method(epi,track); 
    }

     for (size_t i = 0; i < epi->nopi; i++)
        if (track->epi[i] == ESSENTIAL)
            printf("%.*s\n", Kmap->varSize, Kmap->piBin[i]);


    Kmap->piBin = (char **)_2d_arrfree((void **)Kmap->piBin, Kmap->minSize);
    return 0;
}