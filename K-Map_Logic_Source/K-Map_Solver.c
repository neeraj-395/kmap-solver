#include <stdio.h>
#include <conio.h>
#include <string.h>
#include "Self_Define_Functions.h"

void main()
{
  char *MT = (char *)malloc(5);
  char Result[30] = {'\0'};
  int Min_in_Bin[4][2];
  int Stage_1_Output[4][2];
  int row[4];
  int counter;

  printf("Enter the Minterms for Two Varaible K-Map in the respective order\nm0, m1, m2, m3:");
  scanf("%s", MT);

  memset(Result, 0, sizeof(Result));
  memset(Min_in_Bin, 0, sizeof(Min_in_Bin));
  memset(row, 0, sizeof(row));

  BitSetCheck(MT, &Min_in_Bin[0][0], 4, 2);

  if (strcmp(MT, "0000") == 0)
  {
    strcpy(Result, "Logic 0");
  }

  else if (strcmp(MT, "1111") == 0)
  {
    strcpy(Result, "Logic 1");
  }

  else
  {
    if (strcmp(MT, "1001") == 0 || strcmp(MT, "0110") == 0 || Minterm_Counter(&Min_in_Bin[0][0], 4, 2, &row[0]) == 1)
    {
      memcpy(Stage_1_Output, Min_in_Bin, sizeof(Min_in_Bin));
      free(MT);
    }
    else
    {
      for (int N = 0; N < 3; N++)
      {
        for (int M = 0; M < 4; M++)
        {
          if ((N == 0 && (M == 1 || M == 2)) || ((N == 1 || N == 2) && M == 3))
          {
            Comparator(Min_in_Bin, Stage_1_Output, N, M);
          }
        }
      }
    }

    if (Minterm_Counter(&Min_in_Bin[0][0], 4, 2, &row[0]) == 1)
    {
      Boolean_Output(&Stage_1_Output[0][0], 2, &row[0], &Result[0]);
    }
    else
    {
      memset(row, 0, sizeof(row));
      Minterm_Counter(&Stage_1_Output[0][0], 4, 2, &row[0]);
      Boolean_Output(&Stage_1_Output[0][0], 2, &row[0], &Result[0]);
    }
  }
  puts(Result);
  getch();
}