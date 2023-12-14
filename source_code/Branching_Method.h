#ifndef BRANCHING_METHOD_H
#define BRANCHING_METHOD_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

bool covercheck(char *pi, int var, int min);
int optrack(char *track, int size, int *posi);
void piElim(char **table, int nom, int nopi, char *mintrack, char *epitrack);
void minElim(char **table, int nom, int nopi, char *mintrack, char *epitrack);
void cyclicfix(char **table, int nom, int nopi, char *mintrack, char *epitrack);
void extractEPI(char **table, int nom, int nopi, char *mintrack, char *epitrack);

int optrack(char *track, int size, int *posi)
{
    int n = 0;
    for (int i = 0, w = 0; i < size; i++)
    {
        if (track[i] != '.' && track[i] != '#')
        {
            if (posi != NULL)
                posi[w++] = i;
            n++;
        }
    }
    return n;
}

bool covercheck(char *PI, int var, int min)
{
    for (int i = (var - 1); i >= 0; i--)
    {
        if (min >= 0)
        {
            if ((PI[i] - '0') == (min % 2) || PI[i] == '_')
                min >>= 1;
            else
                return false;
        }
    }
    return true;
}

void minElim(char **table, int nom, int nopi, char *mintrack, char *epitrack)
{
    int Adomi, Bdomi, opm[nom], opnom, oppi[nopi], opnopi;
    opnopi = optrack(epitrack, nopi, oppi);
    opnom = optrack(mintrack, nom, opm);

    for (int A = 0; A < opnom; A++)
    {
        for (int B = (A + 1); B < opnom; B++)
        {
            Adomi = 0, Bdomi = 0;
            for (int k = 0; k < opnopi; k++)
            {
                if (table[opm[A]][oppi[k]] == '1')
                    Adomi += (table[opm[B]][oppi[k]] == '0') ? 1 : 0; // mismatch count
                else if (table[opm[B]][oppi[k]] == '1')
                    Bdomi += (table[opm[A]][oppi[k]] == '0') ? 1 : 0; // mismatch count
            }

            if (Adomi == 0 && Bdomi > 0)
                mintrack[opm[B]] = '#';
            else if (Bdomi == 0 && Adomi > 0)
                mintrack[opm[A]] = '#';
            else if (Adomi == 0 && Bdomi == 0)
                mintrack[opm[B]] = '#';
        }
    }
}

void piElim(char **table, int nom, int nopi, char *mintrack, char *epitrack)
{
    int Adomi, Bdomi, opm[nom], opnom, oppi[nopi], opnopi;
    opnom = optrack(mintrack, nom, opm);
    opnopi = optrack(epitrack, nopi, oppi);

    for (int A = 0; A < opnopi; A++)
    {
        for (int B = (A + 1); B < opnopi; B++)
        {
            Adomi = 0, Bdomi = 0;
            for (int k = 0; k < opnom; k++)
            {
                if (table[opm[k]][oppi[A]] == '1')
                    Adomi += (table[opm[k]][oppi[B]] == '0') ? 1 : 0; // mismatch
                else if (table[opm[k]][oppi[B]] == '1')
                    Bdomi += (table[opm[k]][oppi[A]] == '0') ? 1 : 0; // mismatch
            }
            if (Adomi == 0 && Bdomi > 0)
                epitrack[oppi[A]] = '#';
            else if (Bdomi == 0 && Adomi > 0)
                epitrack[oppi[B]] = '#';
            else if (Adomi == 0 && Bdomi == 0)
                epitrack[oppi[A]] = '#';
        }
    }
}

void extractEPI(char **table, int nom, int nopi, char *mintrack, char *epitrack)
{
    int posi = nopi, opm[nom], opnom, oppi[nopi], opnopi; // no. of minterms after elimination
    opnom = optrack(mintrack, nom, opm);
    opnopi = optrack(epitrack, nopi, oppi);

    for (int i = 0, w = 0; i < opnom; i++)
    {
        for (int j = 0, count = 0; j < opnopi; j++)
        {
            if (count > 1)
                break;
            if (table[opm[i]][oppi[j]] == '1')
                posi = ((++count) == 1) ? oppi[j] : nopi;
        }
        if (posi != nopi)
        {
            for (int k = 0; k < opnom; k++)
                if (table[opm[k]][posi] == '1')
                    mintrack[opm[k]] = '#';
            epitrack[posi] = '.';
        }
    }
}

void cyclicfix(char **table, int nom, int nopi, char *mintrack, char *epitrack)
{
    int posi[2];
    for (int i = 0, count; i < nopi; i++)
    {
        if (epitrack[i] != '.' && epitrack[i] != '#')
        {
            count = 0;
            for (int j = 0, w = 0; j < nom; j++)
            {
                if (count > 2)
                    break;
                if (table[j][i] == '1' && mintrack[j] != '#')
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
                mintrack[posi[0]] = '#';
                mintrack[posi[1]] = '#';
                epitrack[i] = '.';
                return;
            }
        }
    }
}

#endif