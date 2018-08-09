#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "parser.h"
#include "map.h"
#include "trie.h"
#include <sys/ioctl.h>
#include <math.h>
#include "heap.h"



int main (int argc, char* argv[]){


    //Variables
    int ch, lines=0, k=0, maximumLineLength = 128, i=0, j=0, twords=0;
    char *doc=NULL, *buffer=NULL;
    double average= 0.0, k1=1.2, b=0.75;
    size_t size= 512;

    read_args(&doc, &k, argc, argv);
    if (k==0 || doc==NULL ){
      fprintf(stderr, "Please, give parameters\n");
      exit(-1);
    }

    FILE *fp = fopen(doc, "r");
      if (fp == NULL){
          fprintf(stderr,"Error opening file\n");
          exit(-1);
      }

      printf("DOC AND K: %s %d \n",doc, k );
      buffer= malloc(sizeof(char) *size);

      while(!feof(fp)){
        ch = fgetc(fp);
        if(ch == '\n'){
          lines++;
        }
      }


      map *array= malloc(sizeof(map)*lines);

      fseek(fp, 0, SEEK_SET); //go to the top of file
      int ret, wrong = 0 , lines_correct=0, q=0;
      char *word;

      if (lines == 0)
        wrong=1;

      for(i=0; i<lines; i++) {
          ret= getline(&buffer, &size, fp);     //takes whole line
          if (ret<0)                            //if line ended
            break;

          buffer[ret-1]= '\0';
          word= strtok(buffer, " \t\n");        //takes id
          if(word == NULL ){                    //just for the last line in document
            lines--;
            continue;
          }

          for(q = 0 ; q < strlen(word) ; q++){  //checking if it's id(=number)
            int temp = word[q];
            if(isdigit(temp)==0){
              printf("there is not an id \n" );
              wrong = 1;
              break;
            }
          }
          if(wrong ==1 )break;
          array[lines_correct].id= atoi(word);
          word = strtok(NULL, "");              //takes only "document"
          if(word == NULL )continue;
          array[lines_correct].line= malloc(sizeof(char)*(strlen(word)+1)); //giati +1?
          strcpy(array[lines_correct].line,word);
          lines_correct++;
      }

      free(doc);
      fclose(fp);

      if( wrong==1){
        printf("%d\n",lines_correct );
        for(int i=0; i<lines_correct; i++){
          free(array[i].line);
        }
        free(array);
        return -1;
      }

      for(j=0; j<lines-1; j++){                 //check numbers sequence
        if( array[j].id >= array[j+1].id){
          printf("%d    %d\n",array[j].id,array[j+1].id );
          printf("Wrong IDs\n");
          for(int i=0; i<lines; i++){
            free(array[i].line);
          }
          free(array);
          return -1;
        }
      }

    TrieNode *trie= createTrieNode('\0');    //create first-basic node
    char* token, *tempLine;

    for( i=0; i<lines; i++){
      array[i].wamount=0;                    //amount of words in the document
      tempLine = malloc((strlen(array[i].line)+1)*sizeof(char));
      strcpy(tempLine,array[i].line);
      token= strtok(tempLine, " ");          //insert every word of every document in trie
      while (token!= NULL){
        insertTrie(trie, token, array[i].id);
        array[i].wamount++;
        twords++;                            //total words
        token= strtok(NULL, " ");
      }
      free(tempLine);
    }
    average= (double)twords/(double)lines;


    Heap* myHeap=CreateHeap();
    // InsertHeap(myHeap,0,1);
    // InsertHeap(myHeap,5,1);
    // InsertHeap(myHeap,7,1);
    // InsertHeap(myHeap,2,1);
    //cleanHeap(myHeap);


    fp = stdin;
    char* tempWord;
    ListNode* tempNode;
    while(!feof(fp)){
      ret = getline(&buffer, &size, fp);      //reading from stdin
      if (ret<0)
        break;
      buffer[ret-1]= '\0';

      tempWord = strtok(buffer," ");
      if(!strcmp(buffer,"\\exit")){
        //printf("BYE\n" );
        break;
      }else if(!strcmp(buffer,"\\search")){
        //printf("Search Operation\n" );
        int count = 0, amount_of_docs=0 ;
        double IDF=0.0, score=0.0;
        ListNode* tempForAmount;
        tempWord = strtok(NULL," ");
        while(tempWord!=NULL && count <10){
          amount_of_docs=0;
          tempNode = tempForAmount = searchTrie(trie,tempWord);
          while (tempForAmount != NULL){
            amount_of_docs++;                   //just to count the amount_of_docs for each word
            tempForAmount= tempForAmount->next;
          }
          IDF= log((double)(lines-amount_of_docs+0.5)/(double)(amount_of_docs+0.5));
          IDF = IDF*(amount_of_docs);
          while (tempNode != NULL){
            score = IDF*((tempNode->freq)*(k1+1))/((tempNode->freq) + k1*(1-b + b*(abs(array[(tempNode->id)].wamount)/average)));
            InsertHeap(myHeap,score,tempNode->id);
            printf("( %d) [%lf] %s\n",tempNode->id,score,array[(tempNode->id)].line);
            tempNode= tempNode->next;
          }


          count++;
          tempWord = strtok(NULL," ");
        }
      }
      else if (!strcmp(buffer, "\\df")) {
        tempWord= strtok(NULL, " ");
        if (tempWord != NULL){
          printWord(trie, word);
        }else{
          int size= 50;
          char* printAllWord;
          printAllWord= malloc(sizeof(char)*size);
          printAll(trie->child,printAllWord,0);
          free(printAllWord);
        }
      }
      else if (!strcmp(buffer, "\\tf")) {
        char* word = strtok(NULL, " ");
        if(word == NULL){
          printf("ID does not exist\n" );
          continue;
        }
        int idd= atoi(word);
        word= strtok(NULL, " ");
        if (word == NULL){
          printf("Word does not exist\n");
          continue;
        }
        tempNode= searchTrie(trie, word);
        while (tempNode!= NULL){
          if(tempNode->id == idd){
            printf("%d  %s  %d\n",idd, word, tempNode->freq);
          }
          tempNode= tempNode->next;
        }
      }
      else{
        printf("Wrong command\n" );
      }
    }//end of while

      for(int i=0; i<lines; i++){
        free(array[i].line);
      }
      free(array);
      free(buffer);
      cleanTrie(&trie);
      cleanHeap(myHeap);
}
