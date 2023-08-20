#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>


int Minterm_Counter(int *Minterms,int sor,int soc,int *row){
  int count=0;
  for(int i=0;i<sor;i++){
      if(*(Minterms + (i * soc))!=2){
        *(row++)=i;
        count++;
    }
  }
  return count;
}


void BitSetCheck(char *Min,int *Min_in_Bin,int sor,int soc){
int Bit;
  for(int i = 0; i < sor; i++){
    if(*(Min++) == '1'){
      Bit=i;
      for(int j = (soc-1); j >= 0; j--){
        *(Min_in_Bin + (i * soc) + j) = Bit%2;
        Bit >>= 1;
    }
  }
  else{
    for(int j = (soc-1); j >= 0; j--){
        *(Min_in_Bin + (i * soc) + j) = 2;
    }
  }
}
}


void Comparator(int Minterms[][2],int Stage_1[][2],int MinPair_N,int MinPair_M){
  static int Row=0;
  int checker=0;
  for(int i = 0; i < 2; i++){
    if(Minterms[MinPair_N][i]==2||Minterms[MinPair_M][i]==2){
      Stage_1[Row][i]=2;
    }
    else if(checker==1){
      Stage_1[Row][i]=0;
    }
    else if(checker==0 && (Minterms[MinPair_N][i])^(Minterms[MinPair_M][i])==1){
      Stage_1[Row][i]=4;
    }
    else{
      Stage_1[Row][i]=Minterms[MinPair_N][i];
    }
  }
  Row++;
}


void Boolean_Output(int *Minterms,int soc,int *row,char *result){
    do{
      *result='(';
        result++; 
        for(int j = 0; j < soc; j++){
            if(*(Minterms + ((*row) * soc) + j)==1){ 
                snprintf(result,sizeof(result),"%cx",65+j);
                result++;
            }
            else if(*(Minterms + ((*row) * soc) + j)==0){
                snprintf(result,sizeof(result),"%c'x",65+j);
                result+=2;
            }
            if (*result=='x')
            {
            strcpy(result," ^ ");
            result+=3;
            }
        }
        if(*(result-2)=='^') result-=3;
        strcpy(result,")  ");
        result++;
        ++row;
        if(*row!=0 && *result!='\0'){
        strcpy(result," V ");
        result+=3;
        }
        else{ 
        *result='\0';
        break;
        }
    }while(*row !=0);
}


void ArrayDisplay(int *Array,int sor,int soc,char ArrayName[10]){
  for(int i = 0; i < sor; i++){
    for(int j = 0; j < soc; j++){
      printf("%s[%d][%d] = %d, ",ArrayName,i,j,*(Array + i * soc + j));
    }
    printf("\n");
  }
}