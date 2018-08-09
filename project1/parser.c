#include <stdio.h>
#include <string.h>
#include <stdlib.h>




//Arguments Function
void read_args(char** document,int* results,int argc,char* argv[]){
  int i;
  for(i=0 ; i< argc ; i++){
    if(!strcmp(argv[i],"-i")){
      *document= malloc(sizeof(char)*(strlen(argv[i+1])+1));
      strcpy(*document, argv[i+1]);
    }
    if(!strcmp(argv[i],"-k")){
      *results=atoi(argv[i+1]);
    }
  }
}
