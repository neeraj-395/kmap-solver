#ifndef BRANCHING_METHOD_H
#define BRANCHING_METHOD_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tabular_method.h"
#define ESSENTIAL '.'
#define NON_ESSENTIAL '#'
#define COVERED '#'
#define UNCOVERED '$'

typedef struct prime_implicant_table
{
    char **table;   /* prime implicant table, a two dimentional array! */
    size_t nom;     /* number of minterms */
    size_t nopi;    /* number of prime implicant */
} pit_set;

typedef struct covered_terms_track
{
    char *min;
    char *epi;
} ctt_set;



/* bool draw_pitable(char *pi, int var, int min);
int uncovered_terms(char *track, int size, int *posi);
void non_essential_primes_eliminator(pit_set *epi, ctt_set *track);
void covered_minterms_eliminator(pit_set *epi, ctt_set *track);
void cyclic_function_method(pit_set *epi, ctt_set *track);
void collect_essential_primes(pit_set *epi, ctt_set *track);*/



size_t uncovered_terms(char *track, size_t size, size_t *posi)
{
    size_t n = 0;
    for (size_t i = 0, w = 0; i < size; i++)
    {
        if (track[i] == UNCOVERED)
        {
            if (posi)
                posi[w++] = i;
            n++;
        }
    }
    return n;
}

void draw_pitable(pit_set *epi)
{
    for (size_t i = 0; i < epi->nom; i++)
    {
        for (size_t j = 0; j < epi->nopi; j++)
        {
            size_t min_dec = Kmap->minDec[i];
            bool check = true;
            for (int k = (Kmap->varSize - 1); k >= 0; k--)
            {   
                if ((Kmap->piBin[j][k] - '0') == (int)(min_dec % 2) 
                                  || Kmap->piBin[j][k] == '_')
                    min_dec >>= 1;
                else{
                    check = false;
                    break;
                }
            }
            epi->table[i][j] = (check) ? '1' : '0';
        }
    }
    free(Kmap->minDec);
    Kmap->minDec = NULL;
}

void covered_minterms_eliminator(pit_set *epi, ctt_set *track)
{
    size_t Adomi, Bdomi;
    size_t opm[epi->nom], opnom;
    size_t oppi[epi->nopi], opnopi;

    opnopi = uncovered_terms(track->epi, epi->nopi, oppi);
    opnom = uncovered_terms(track->min, epi->nom, opm);

    for (size_t A = 0; A < opnom; A++)
    {
        for (size_t B = (A + 1); B < opnom; B++)
        {
            Adomi = 0, Bdomi = 0;
            for (size_t k = 0; k < opnopi; k++)
            {
                if (epi->table[opm[A]][oppi[k]] == '1')
                    Adomi += (epi->table[opm[B]][oppi[k]] == '0') ? 1 : 0; // mismatch count
                else if (epi->table[opm[B]][oppi[k]] == '1')
                    Bdomi += (epi->table[opm[A]][oppi[k]] == '0') ? 1 : 0; // mismatch count
            }

            if (Adomi == 0 && Bdomi > 0)
                track->min[opm[B]] = COVERED;
            else if (Bdomi == 0 && Adomi > 0)
                track->min[opm[A]] = COVERED;
            else if (Adomi == 0 && Bdomi == 0)
                track->min[opm[B]] = COVERED;
        }
    }
}

void non_essential_primes_eliminator(pit_set *epi, ctt_set *track)
{
    size_t Adomi, Bdomi;
    size_t opm[epi->nom], opnom;
    size_t oppi[epi->nopi], opnopi;

    opnom = uncovered_terms(track->min, epi->nom, opm);
    opnopi = uncovered_terms(track->epi, epi->nopi, oppi);

    for (size_t A = 0; A < opnopi; A++)
    {
        for (size_t B = (A + 1); B < opnopi; B++)
        {
            Adomi = 0, Bdomi = 0;
            for (size_t k = 0; k < opnom; k++)
            {
                if (epi->table[opm[k]][oppi[A]] == '1')
                    Adomi += (epi->table[opm[k]][oppi[B]] == '0') ? 1 : 0; // mismatch
                else if (epi->table[opm[k]][oppi[B]] == '1')
                    Bdomi += (epi->table[opm[k]][oppi[A]] == '0') ? 1 : 0; // mismatch
            }
            if (Adomi == 0 && Bdomi > 0)
                track->epi[oppi[A]] = NON_ESSENTIAL;
            else if (Bdomi == 0 && Adomi > 0)
                track->epi[oppi[B]] = NON_ESSENTIAL;
            else if (Adomi == 0 && Bdomi == 0)
                track->epi[oppi[A]] = NON_ESSENTIAL;
        }
    }
}

void collect_essential_primes(pit_set *epi, ctt_set *track)
{
    size_t posi = epi->nopi;
    size_t opm[epi->nom], opnom;
    size_t oppi[epi->nopi], opnopi; // no. of minterms after elimination

    opnom = uncovered_terms(track->min, epi->nom, opm);
    opnopi = uncovered_terms(track->epi, epi->nopi, oppi);

    for (size_t i = 0; i < opnom; i++)
    {
        for (size_t j = 0, count = 0; j < opnopi; j++)
        {
            if (count > 1)
                break;
            if (epi->table[opm[i]][oppi[j]] == '1')
                posi = ((++count) == 1) ? oppi[j] : epi->nopi;
        }
        if (posi != epi->nopi)
        {
            for (size_t k = 0; k < opnom; k++)
                if (epi->table[opm[k]][posi] == '1')
                    track->min[opm[k]] = COVERED;
            track->epi[posi] = ESSENTIAL;
        }
    }
}

void cyclic_function_method(pit_set *epi, ctt_set *track)
{
    size_t posi[2];
    for (size_t i = 0, count; i < epi->nopi; i++)
    {
        if (track->epi[i] == UNCOVERED)
        {
            count = 0;
            for (size_t j = 0; j < epi->nom; j++)
            {
                if (count > 2)
                    break;
                if (epi->table[j][i] == '1' && track->min[j] == UNCOVERED)
                {
                    count++;
                    if (count == 1)
                        posi[0] = j;
                    else if (count == 2)
                        posi[1] = j;
                }
            }
            if (count == 2)
            {
                track->min[posi[0]] = COVERED;
                track->min[posi[1]] = COVERED;
                track->epi[i] = ESSENTIAL;
                return;
            }
        }
    }
}

#endif