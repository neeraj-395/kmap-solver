#include <stdio.h>
#include <conio.h>
#include <string.h>
void Expression(char *p,char *r){
  for(int i=0;i<4;i++){
  if(*(p+i) == '1'){
    if(i>=1){
    strcpy(r," V ");
    r+=3;
    }
    if(i==0){
    strcpy(r,"(A' ^ B')");
    r+=9;
    }
    if(i==1){
    strcpy(r,"(A' ^ B)");
    r+=8;
    }
    if(i==2){
    strcpy(r,"(A ^ B')");
    r+=8;
    }
    if(i==3){
    strcpy(r,"(A ^ B)");
    r+=7;
    }
  }
  } 
  };
void main(){
char Y[2][2]={{'1','0'},{'1','1'}},Result[30];
Expression(&Y[0][0],&Result[0]);
puts(Result);
getch();
}

