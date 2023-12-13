#include "Quine_McCluskey.h"
#include "Branching_Method.h"
#define MAX_VAR 5

int main(int argc, char const *MinChar[])
{
    if (argc <= 1)
    {
        printf("No input has been received.");
        return 1;
    }

    int Min;                               // Number of Minterms including 'X'
    int Var;                               // Number of Variables
    int _1sC;                              // Number of One's
    const int inSize = strlen(MinChar[1]); // Number of input characters
    float temp = log2f((float)inSize);

    Min = CC(MinChar[1], inSize, '1', 'X');  // Counting number of minterms including 'X'
    _1sC = CC(MinChar[1], inSize, '1', '$'); // Counting number of one's

    if (!Min || !_1sC)                       // If no. of minterms or number of 1's is equal to 0
    {
        FILE *EPI_file = fopen("EPI.txt", "w");
        fprintf(EPI_file, "%c", '0');        // then return output '0' program end
        fclose(EPI_file);
        return 0;
    }
    else if ((int)temp == temp && (int)temp <= MAX_VAR) // If floating value of log2f(inSize) = integer value of itself
        Var = (int)temp; // then assign log2f(inSize) to number of one's
    else
    {
        printf("Number of input characters (0,1,X) should be 2^n where n is (2 >= n =< 5).");
        return 1; // this program supports upto 5 variables if more than that return 1 program breaks!!
    }

    //   XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //   X * Prime Implicants Identification Implimentation Using Quine McCluskey Algorithm.  X
    //   XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    char PI[inSize][Var];                             // Prime Implicants Storage!!
    int Lpp = (Var * pow(2, Var - 1));                // The largest possible pair (When all Minterms is set to '1' or The Worst case of this program)
    int PIndex, sizeOpp;                              // Prime Implicant Index, Size of possible pair
    int *Pp = (int *)malloc((Lpp * 2) * sizeof(int)); // Allocating Memory for Possible-Pairs
    char *tick = (char *)malloc(Lpp * sizeof(char));  // Ticked and UnTicked Minterms Track stored here
    char **Min_in_Bin = _2DPointer(Min, Var);         // Minterms in Binary form stored here

    memset(tick, '$', sizeof(char) * Lpp); // Setting Whole tick array with '$' Character

    Dec_to_Bin(MinChar[1], inSize, Var, Min_in_Bin); // Converting Input Data to its Corresponding Binary Form

    int C0Min = Min; // Clone-0 Minterm-Count
    int C1Min = Min; // Clone-1 Minterm-Count

    while (1) // Infinite Loop
    {
        memset(tick, '0', sizeof(char) * C1Min); // Assuming all the minterms are prime implicants!!

        sizeOpp = PT_Generator(Min_in_Bin, C1Min, Var, Pp, tick); // Generating Possible Pairs And Ticking PI!!

        PIndex = Prime_Implicant(Min_in_Bin, &PI[0][0], Var, tick, Lpp); // Storing Generated PI in the PI Array!!

        if (sizeOpp == 0)
            break; // Exit the loop when thier is no Possible Pairs is Available!!

        else if (sizeOpp < Lpp)
            Pp = (int *)realloc(Pp, (sizeOpp * 2) * sizeof(int)); // Freeing the Unuseble memory space!!

        C0Min = sizeOpp;

        char **Temp = _2DPointer(C0Min, Var); // Temporary Storage for Minterms Comparision!!

        //------------------------Comparision Stage Between Paired Minterms--------------------------//
        {
            for (int i = 0; i < C0Min; i++)
            {
                for (int j = 0; j < Var; j++)
                {
                    if ((Min_in_Bin[Pp[(2 * i)]][j] ^ Min_in_Bin[Pp[(2 * i) + 1]][j]) == 1)
                        Temp[i][j] = '_';
                    else
                        Temp[i][j] = Min_in_Bin[Pp[(2 * i)]][j];
                }
            }
        }
        //-------------------------------------Comparision End--------------------------------------//

        //------------Data Exchanging Process------------//
        {
            _2DFreeArray(Min_in_Bin, C1Min);     // Freeing the Binary Array Data Memory Space!!
            Min_in_Bin = _2DPointer(C0Min, Var); // Creating Again 2D Pointer Array with Changed Minterm Sized!!
            {
                for (int i = 0; i < C0Min; i++)
                    memcpy(Min_in_Bin[i], Temp[i], sizeof(char) * Var); // Copying Data Temp to Min_in_Bin Array!!
            }
            _2DFreeArray(Temp, C0Min); // Freeing Temp Array
        }
        //--------------End of Data Exchange------------//

        memset(tick, '$', sizeof(char) * C1Min); // Resetting tick Data with dollar characters!!

        C1Min = C0Min; // Copying Changed Minterms-Count to Other Minterm Count!!
    }

    _2DFreeArray(Min_in_Bin, C1Min); // Freeing Min_in_Bin-Clone After Loop End!!

    free(Pp);   // freeing Possible Pair Memory After Loop End!!
    free(tick); // freeing tick Memory space!!

    //   XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //   X * Essential Prime Implicants Identification Implimentation Using Dominance Method. X
    //   X * For Cyclic Boolean Functions Branching Method is Used.                           X
    //   XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    int nom = _1sC;                         // Number of minterms execluding 'X'
    int nopi = PIndex;                      // Number of Prime Implicants for dominance method
    int MinD[nom];                          // Minterms in decimals
    char **pitable = _2DPointer(nom, nopi); // Prime implicant table for boolean function simplification

    for (int i = 0, w = 0; i < inSize; i++)
        if (MinChar[1][i] == '1')
            MinD[w++] = i;                  // Generating Minterms in decimal form

    for (int i = 0; i < nom; i++)
    {
        for (int j = 0; j < nopi; j++)
        {
            if (covercheck(&PI[j][0], Var, MinD[i]))
                pitable[i][j] = '1';
            else
                pitable[i][j] = '0';
        }
    }
    //---------------End of Creation----------------//

    char mintrack[nom];  // it will track those minterm which are eliminated by using the symbol '#'
    char epitrack[nopi]; // it will track those prime implicants which are non-essential by symbol '#' and '.' for essential.
    int epinx = 0;       // number of essential prime implicants

    memset(mintrack, '$', sizeof(char) * nom);  // Setting whole mintrack array with '$' sign
    memset(epitrack, '$', sizeof(char) * nopi); // Setting whole epitrack array with '$' sign

    while (optrack(mintrack, nom, NULL))
    {
        minElim(pitable, nom, nopi, mintrack, epitrack);    // Eliminating those minterms which are covered.
        piElim(pitable, nom, nopi, mintrack, epitrack);     // Eliminating those prime implicants which are non-essential.
        extractEPI(pitable, nom, nopi, mintrack, epitrack); // extracting prime implicants that are essential.
        if (optrack(mintrack, nom, NULL))
            cyclicfix(pitable, nom, nopi, mintrack, epitrack); // branching method execution for cyclic functions
    }

    FILE *EPI_file = fopen("EPI.txt", "w");
    for (int i = 0; i < nopi; i++)
    {
        if (epitrack[i] == '.')
        {
            fwrite(&PI[i][0], sizeof(char), Var, EPI_file);
            fprintf(EPI_file, "%c", '\n');
        }
    }


    fclose(EPI_file);// freeing pointer

    _2DFreeArray(pitable, nom); // freeing memory space 

    return 0;
}
