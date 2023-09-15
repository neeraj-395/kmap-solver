#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

//----------------Decimal to Binary Coverter-------------------//
void BitSetCheck(char *MinC, char *MinB, size_t *sor, size_t soc)
{
  int Bit;
  int Row = 0;
  for (int i = 0; i < *sor; i++)
  {
    if ((*(MinC + i) == '1') || (*(MinC + i) == 'X'))
    {
      Bit = i;
      for (int j = (soc - 1); j >= 0; j--)
      {
        *(MinB + (Row * soc) + j) = (char)(48 + (Bit % 2));
        Bit >>= 1;
      }
      Row++;
    }
  }
  *sor = Row;
}

//----------------Binary to Bool Algebraic Output------------------------//
void Boolean_Output(char *Minterms, size_t soc, size_t *row, char *result)
{
  do
  {
    *result = '(';
    result++;
    for (int j = 0; j < soc; j++)
    {
      if (*(Minterms + ((*row) * soc) + j) == '1')
      {
        snprintf(result, sizeof(result), "%cx", 65 + j);
        result++;
      }
      else if (*(Minterms + ((*row) * soc) + j) == '0')
      {
        snprintf(result, sizeof(result), "%c'x", 65 + j);
        result += 2;
      }
      if (*result == 'x')
      {
        strcpy(result, " ^ ");
        result += 3;
      }
    }
    if (*(result - 2) == '^')
      result -= 3;
    strcpy(result, ")  ");
    result++;
    ++row;
    if (*row != 0 && *result != '\0')
    {
      strcpy(result, " V ");
      result += 3;
    }
    else
    {
      *result = '\0';
      break;
    }
  } while (*row != 0);
}

//--------------2D-Pointer Array Display Function(Temporary)------------//
void _2D_Ptr_Display(char **Arr, size_t sor, size_t soc, char ArrName[10])
{
  for (int i = 0; i < sor; i++)
  {
    for (int j = 0; j < soc; j++)
    {
      printf("%s[%d][%d] = %c, ", ArrName, i, j, Arr[i][j]);
    }
    printf("\n");
  }
}

//-----------------------Diff_Set_Counter-------------------------//
int DSC(char **MinB, size_t soc, size_t M, size_t N, char *Set_count)
{
  int diff_count = 0;
  int set_I = 0, set_II = 0;
  for (int i = 0; i < soc; i++)
  {
    if (MinB[M][i] != MinB[N][i])
      diff_count++;
    if (MinB[M][i] == '1')
      set_I++;
    if (MinB[N][i] == '1')
      set_II++;
  }
  if (set_I == (set_II - 1))
    *Set_count = '1';

  else
    *Set_count = '0';

  return diff_count;
}

//------------------------Pair & Tick Generator--------------------------//
int PT_Generator(char **MinB, size_t sor, size_t soc, int *PP, char Tick[])
{
  int PProw = 0;
  char set_count;
  for (int M = 0; M < sor; M++)
  {
    for (int N = (M + 1); N < sor; N++)
    {
      if (DSC(MinB, soc, M, N, &set_count) == 1)
      {
        *(PP + (PProw * 2)) = M;
        *(PP + (PProw * 2) + 1) = N;
        Tick[M] = '1';
        Tick[N] = '1';
        PProw++;
      }
      else if (set_count == '1')
      {
        if (Tick[M] != '1')
          Tick[M] = '0';
        if (Tick[N] != '1')
          Tick[N] = '0';
      }
      else if (PProw == 0)
      {
        Tick[M] = '0';
        Tick[N] = '0';
      }
    }
  }
  return PProw;
}

//-------------------Prime_Implicant_Collector------------------//
int Prime_Implicant(char **MinB, char *PI, size_t soc, char Tick[])
{
  static int Line = 0;
  int i = 0, check = 0;
  while (Tick[i] != '\0')
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

//---Initializing Dynamic_2D-Array----//
char **_2DPointer(size_t Rows, size_t Col)
{

  char **Array;
  Array = (char **)malloc(Rows * sizeof(char *));

  for (int i = 0; i < Rows; i++)
    Array[i] = (char *)malloc(Col * sizeof(char));

  return Array;
}

//--------Free The Allocated Array-------//
void _2DFreeArray(char **Array, size_t Rows)
{
  for (int i = 0; i < Rows; i++)
    free(Array[i]);

  free(Array);
}