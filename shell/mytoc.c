#include "mytoc.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// Function to Copy a Strig into the Vector
char *stringCopy(char *str, int len, int index){

  char *aCopy = (char*) malloc(1024);
  int count=0;
  for(int i=index ;i<(index+len) ;i++){
    aCopy[count]= str[i];
    count++;
  }
  aCopy[len]=0;
  return aCopy;
}


// This method will count the number of tokens in the string
int numTokens(char* str, char delim){
  int  isSpace=1, wCount=0;
  
 for(int i =0; str[i]!='\0'; i++){
    if(str[i]==delim){
      isSpace=1;
    }
     if(isSpace==1 && str[i]!=delim){
         wCount++;
      isSpace=0;
    }
     
}
 return wCount;
}


//Function main for Toc
char ** myToc(char* str, char delim){
  int vSize,len=0, wCount=0, isSpace=1,index=0;
  wCount= numTokens(str,delim);
  //  printf("Number of Tokens: %d\n", wCount);
   //Vector that will have all the tokens adress 
  char **tokenVec = (char**) calloc(wCount+1, sizeof(char*));
  tokenVec[wCount]=0;
   int word=0;
   if(wCount==0){
     char **emptyVec = (char**) calloc(20,sizeof(char*));
     emptyVec[0]= "Command Not Entered!";
     emptyVec[1]= '\0';
     return emptyVec;
   }
   for(int i =0; str[i]!='\0'; i++){
       if(str[i]==delim){
      isSpace=1;
    }
     if(isSpace==1 && str[i]!=delim){
        word++;
	index=i;
	len=0;
	isSpace=0;
	//printf("Len: %d%c\n", len,str[i]);
    }
     if(isSpace==0 && str[i] !=delim){
       len++;
       
       //printf("Len: %d%c\n", len,str[i]);
        
     }
     if(isSpace==0 && str[i+1]==delim|| str[i+1]=='\0'){
       
       tokenVec[word-1]=(char*)malloc(len+1);
        tokenVec[word-1]= stringCopy(str,len,index);
          }

  
   
     
    
    tokenVec[wCount]= 0;
   } // end of FOR LOOP

   return tokenVec;
}// end of myToc function


/*int main(){
  myToc(" Hello p p p p   ", ' ');

  } */
