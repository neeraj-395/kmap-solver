#include <stdio.h>
#include <conio.h>
#include <string.h>
void Expression(char *p,char *r){
  if(*p == '1'){
   strcpy(r,"A' ^ B' + ");
   r+=10;
  }
  p++;
  if(*p == '1'){
    strcpy(r,"A ^ 'B + ");
   r+=10;
  }
  p++;
  if(*p == '1'){
    strcpy(r,"A ^ B' + ");
   r+=10;
  }
  p++;
  if(*p == '1'){
    strcpy(r,"A ^ B ");
  }
  };
void main(){
char Y[2][2]={{'1','0'},{'0','1'}},Result[30];
Expression(&Y[0][0],&Result[0]);
puts(Result);
getch();
}

