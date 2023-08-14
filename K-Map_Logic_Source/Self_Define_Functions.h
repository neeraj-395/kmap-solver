#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
int Minterm_Counter(int Minterms[][2],int *row){
  int count=0;
  for(int i=0;i<4;i++){
      if(Minterms[i][0]!=2){
        *(row++)=i;
        count++;
    }
  }
  return count;
}

void BitSetCheck(char *Min,int Min_in_Bin[][2]){
  for(int i=0;i<4;i++){
    if(*(Min++) == '1'){
      int M=i;
      for(int j=1;j>=0;j--){
        Min_in_Bin[i][j] = M%2;
        M/= 2;
    }
  }
  else{
    for(int j=1;j>=0;j--){
        Min_in_Bin[i][j] = 2;
    }
  }
}
}

void Comparator(int Minterms[][2],int Stage_1[][2],int MinPair_N,int MinPair_M){
  static int Line=0;
  int checker=0;
  for(int i = 0; i < 2; i++){
    if(Minterms[MinPair_N][i]==2||Minterms[MinPair_M][i]==2){
      Stage_1[Line][i]=2;
    }
    else if(checker==1){
      Stage_1[Line][i]=0;
    }
    else if(checker==0 && (Minterms[MinPair_N][i])^(Minterms[MinPair_M][i])==1){
      Stage_1[Line][i]=4;
    }
    else{
      Stage_1[Line][i]=Minterms[MinPair_N][i];
    }
  }
  Line++;
}

void Boolean_Output(int Minterms[][2],int *row,char *result){
    int checker;
    for(int i = 0; i < 2; i++){
      checker=0;
        for(int j = 0; j < 2; j++){
            if(Minterms[*row][j]==1){
                if(j==0){
                *result='A';
                result++;
                }
                else{
                *result='B';
                result++;
                }
                checker++;
            }
            else if(Minterms[*row][j]==0){
                if(j==0){
                    strcpy(result,"A'");
                    result+=2;
                }
                else{
                   strcpy(result,"B'");
                   result+=2;
                }
                checker++;
            }

            if (Minterms[*row][j+1]!=4 && Minterms[*row][j+1]!=2 && j<1 && checker==1)
            {
            strcpy(result," ^ ");
            result+=3;
            }
            
        }
        ++row;
        if(*row!=0 && (checker==1||checker==2)){
        strcpy(result," V ");
        result+=3;
        }
        else break;
    }
}

char* concatenate(const char arr[][2], int rows, int cols) {
    int Elements = rows * cols;
    char *concatenated = (char *)malloc(Elements + 1);

    if (concatenated == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    int index = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            concatenated[index++] = arr[i][j];
        }
    }

    concatenated[Elements] = '\0';

    return concatenated;
}