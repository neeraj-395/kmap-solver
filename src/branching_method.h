#ifndef BRANCHING_METHOD_H
#define BRANCHING_METHOD_H
#include <stdio.h>
#include <stdbool.h>
#define ESSENTIAL '.'
#define NON_ESSENTIAL '#'
#define COVERED '#'
#define UNCOVERED '$'
#define ULL_INT unsigned long long int

typedef struct prime_implicant_table_set
{
    char **table;   /* prime implicant table, a two dimentional array! */
    size_t nom;     /* number of minterms */
    size_t nopi;    /* number of prime implicant */
} pit_set;

typedef struct covered_terms_track_set
{
    char *min;
    char *epi;
} ctt_set;

void draw_pitable(pit_set *epi, char **piBin, ULL_INT *minDec, size_t var);
void cyclic_function_method(pit_set *epi, ctt_set *track);
void collect_essential_primes(pit_set *epi, ctt_set *track);
size_t uncovered_terms(char *track, size_t size, size_t *posi);
void covered_minterms_eliminator(pit_set *epi, ctt_set *track);
void non_essential_primes_eliminator(pit_set *epi, ctt_set *track);

#endif