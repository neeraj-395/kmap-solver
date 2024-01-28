#ifndef QUINE_MCCLUSKEY_H
#define QUINE_MCCLUSKEY_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

char **_2DPointer(int Rows, int Col);
void _2DFreeArray(char **Array, int Rows);
bool DE(char **MinB, int soc, int M, int diff_posi);
int CC(char const *MinC, int sor, char ch0, char ch1);
int DC(char **MinB, int soc, int M, int N, int *diff_posi);
void Dec_to_Bin(char const *MinC, int sor, int soc, char **MinB);
int PT_Generator(char **MinB, int sor, int soc, int *PP, char Tick[]);
int Prime_Implicant(char **MinB, char *PI, int soc, char Tick[], int soT);

void Dec_to_Bin(char const *MinC, int sor, int soc, char **MinB)
{
  int Bit;
  for (int i = 0, Row = 0; i < sor; i++)
  {
    if ((MinC[i] == '1') || (MinC[i] == 'X'))
    {
      Bit = i;
      for (int j = (soc - 1); j >= 0; j--)
      {
        MinB[Row][j] = (char)(48 + (Bit % 2));
        Bit >>= 1;
      }
      Row++;
    }
  }
}

int CC(char const *MinC, int sor, char ch0, char ch1)
{
  int count = 0;
  for (int i = 0; i < sor; i++)
    if (MinC[i] == ch0 || MinC[i] == ch1)
      count++;
  return count;
}

bool DE(char **MinB, int soc, int M, int diff_posi)
{
  for (int i = 0; i < soc; i++)
  {
    if (MinB[M][i] == '_')
    {
      if (diff_posi > i)
        return false;
    }
  }
  return true;
}

int DC(char **MinB, int soc, int M, int N, int *diff_posi)
{
  int diff_count = 0;
  for (int i = 0; i < soc; i++)
  {
    if (MinB[M][i] != MinB[N][i])
    {
      diff_count++;
      *diff_posi = (int)i;
    }
  }
  return diff_count;
}

int PT_Generator(char **MinB, int sor, int soc, int *PP, char Tick[])
{
  int PProw = 0, diff_posi;
  for (int M = 0; M < sor; M++)
  {
    for (int N = (M + 1); N < sor; N++)
    {
      if (DC(MinB, soc, M, N, &diff_posi) == 1)
      {
        if (DE(MinB, soc, M, diff_posi))
        {
          *(PP + (PProw * 2)) = M;
          *(PP + (PProw * 2) + 1) = N;
          PProw++;
        }
        Tick[M] = '1';
        Tick[N] = '1';
      }
    }
  }
  return PProw;
}

int Prime_Implicant(char **MinB, char *PI, int soc, char Tick[], int soT)
{
  static int Line = 0;
  int i = 0;
  while ((Tick[i] != '$') && (i != soT))
  {
    if (Tick[i] == '0')
    {
      strncpy(&PI[Line * soc], &MinB[i][0], sizeof(char) * soc);
      Line++;
    }
    i++;
  }
  return Line;
}

char **_2DPointer(int Rows, int Col)
{
  if (Rows < 1 || Col < 1)
    return NULL;
  char **Array;
  Array = (char **)malloc(Rows * sizeof(char *));

  for (int i = 0; i < Rows; i++)
    Array[i] = (char *)malloc(Col * sizeof(char));

  return Array;
}

void _2DFreeArray(char **Array, int Rows)
{
  for (int i = 0; i < Rows; i++)
    if (Array[i] != NULL)
      free(Array[i]);

  if (Array != NULL)
    free(Array);
}

#endif