#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "Implement.h"

void main()
{
  size_t Min = 8; // Number of Minterms
  size_t Var = 3; // Number of Variables

  char Min_in_Char[] = {'1', '1', '1', '1', '1', '1', '0', '1' /*, '1', '0', '0', '0', '1', '1', '0', '1'*/};

  char Min_in_Bin[Min][Var]; // The Binary Form of the Minterms will be stored here!!

  int PI_CLine, PP_ELine = 32; // Prime Implicant Current Line, Possible Pair End Line

  int *PP = (int *)malloc((PP_ELine * 2) * sizeof(int)); // Allocated Memory for Possible-Pairs

  char Result[60] = {'\0'}; // Result wiil be stored here in Boolean Algebraic form

  char Tick[16]; // Ticked and UnTicked Minterms Track stored here

  char PI[Min][Var]; // Prime Implicants Container!!

  memset(Tick, '$', sizeof(Tick)); // Setting Whole Array with Doller Character
  memset(PI, '\0', sizeof(PI));    // Setting Whole Array with Null character

  BitSetCheck(&Min_in_Char[0], &Min_in_Bin[0][0], &Min, Var); // Converting Input Data to its Corresponding Binary Form

  size_t C0Min = Min; // Clone-0 Minterm-Count
  size_t C1Min = Min; // Clone-1 Minterm-Count

  char **MinB_Clone = _2DPointer(C1Min, Var); // Binary Minterms Clone for Further Operations Execution

  for (int i = 0; i < C1Min; i++)
    strncpy(&MinB_Clone[i][0], &Min_in_Bin[i][0], sizeof(char) * Var); // Copying Orignal Data to Clone Container

  while (1) // Infinite Loop
  {
    PP_ELine = PT_Generator(MinB_Clone, C1Min, Var, PP, Tick); // Generating Possible Pairs And Ticking PI!!

    PI_CLine = Prime_Implicant(MinB_Clone, &PI[0][0], Var, Tick); // Storing Generated PI in the PI-Container!!

    if (PP_ELine < 32)
      PP = (int *)realloc(PP, (PP_ELine * 2) * sizeof(int)); // Freeing the Unuseble memory space!!

    if (PP_ELine == 0)
      break; // Exit the loop when thier is no Possible Pair Available!!

    C0Min = PP_ELine;

    char **Temp = _2DPointer(C0Min, Var); // Temporary Container for Minterms Comparision!!

    //------------------------Comparision Stage Between Paired Minterms--------------------------//
    {
      int Line = 0;
      for (int i = 0; i < C0Min; i++)
      {
        for (int j = 0; j < Var; j++)
        {
          if ((MinB_Clone[*(PP + (2 * i))][j] ^ MinB_Clone[*(PP + (2 * i) + 1)][j]) == 1)
          {
            Temp[Line][j] = '_';
          }
          else
          {
            Temp[Line][j] = MinB_Clone[*(PP + (2 * i))][j];
          }
        }
        Line++;
      }
    }
    //-------------------------------------Comparision End--------------------------------------//

    //------------Data Exchanging Process------------//
    {
      _2DFreeArray(MinB_Clone, C1Min);     // Freeing the Binary Clone Data Memory Space!!
      MinB_Clone = _2DPointer(C0Min, Var); // Creating Again 2D Pointer Array with Changed Minterm Sized!!

      {
        for (size_t i = 0; i < C0Min; i++)
        {
          for (size_t j = 0; j < Var; j++)
          {
            MinB_Clone[i][j] = Temp[i][j]; // Copying Data Temp to Clone Array!!
          }
        }
      }
      _2DFreeArray(Temp, C0Min); // Freeing Temp Array
    }
    //-------------End of Data Exchange------------//

    memset(Tick, '$', sizeof(Tick)); // Resetting Tick Data with dollar characters!!

    C1Min = C0Min; // Copying Changed Minterms-Count to Other Minterm Count!!
  }

  _2DFreeArray(MinB_Clone, C1Min); // Freeing Min_in_Bin-Clone After Loop End!!

  free(PP); // Freeing Possible Pair Memory After Loop End!!

  for (int i = 0; i < Min; i++)
  {
    printf("%c%c%c\n", PI[i][0], PI[i][1], PI[i][2]); // Printing Generated Prime Implicant!!
  }
}

/*"This program is currently incomplete and can only print the prime implicant,
which does not represent the final or accurate result of Boolean simplification.
The generation of essential prime implicants for the final result is a feature that will be updated in the near future.
Therefore, I recommend using the previous version of this program,
which can be downloaded from the development history file containing the download link."*/