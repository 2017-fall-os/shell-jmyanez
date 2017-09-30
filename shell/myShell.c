


/*
Code created by: Jose Yanez
Finishd: 9/24/2017-:-8:50 PM MT
 */
#include <stdio.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytoc.c"
#include "myShell.h"

// This will recieve the enviromental variable to look for the path
char* getPath(char **envp){
  for(int i=0; envp[i]!= '\0';i++){
    char **vect = myToc(envp[i],'=');
    if(stringComp(vect[0],"PATH")){
      return vect[1];
      }
  }
 
return NULL;
}

// Function that will work to count characters in a string
int countChar(char* aString){
  int count;
  for(int i =0 ; aString[i]!='\0';i++){
    
    count++;
  }
  return count;
}


//String comparer implemented to compare 2 strings

int stringComp(char* cmd1, char* cmd2){
  for(int i=0;cmd1[i]!='\0' || cmd2[i]!='\0' ;i++){
    
    if(cmd1[i] != cmd2[i]){ 
      return 0;
    }//If statx
  }//For loop
  
  return 1;
}



//This function will be used to concatenate 2 strings into 1
//This will be needed to check the paths with the command

char* concString(char* s1, char* s2){
  char* newString = (char*)malloc(100);

  int i;
  for( i=0; s1[i] != '\0'; i++){
    newString[i]= s1[i];
  }
  newString[i]='/';                         ///////// puts the slash for the directory but there could be a better way
                                            //////// To do it in the main  this is just for meeting the deadline.   
  i++;
  for(int j =0; s2[j] != '\0';j++,i++){
    newString[i]=s2[j]; 
  }
  newString[i] = '\0';


  return newString;
}

int cdCheck(char* command, char** tVec){
if(stringComp(command,"cd")){
       if(stringComp(tVec[1],"..")){
      printf("CHANGE DIR\n");

      	}
    }
}


// Shell main functin

int main(int arc, char **argv, char **envp){
 
  char cmd1[60];
  int out  =0, pID, found=0; // verifies if te program keeps running
  char **tVec;//This will save the tokenized strings
  char **paths,  *path;
  struct stat buf;
  while(out!=1){

    char *command = NULL;    
    
    write(1,"$\n",1);
    scanf("%[^\n]%*c", cmd1);
    out = stringComp(cmd1,"exit");
    tVec= myToc(cmd1,' ');
    
    command= tVec[0];
    
    path =getPath(envp);// This will call the getPath function and returns a string with all the paths
    paths=myToc(path,':');// This will separate all the paths possibles for the command by ':'

    if(cdCheck(command,tVec)){

    }


    else if(tVec[1]!='\0' && tVec[1]=='|'){

      printf("La Pipa de la paz");


    }
    
    //Check the command with stat() system call.
    //This first stat call will asume that you were given the path
    //in case of failling we will try the aproach of looking trough all the paths
    else if(stat(command,&buf) == 0 ){
      write(1,"Command Found!\n",15);
      found=1;
      pID = fork();

      
      if(pID < 0){
      perror("Fork Error");
      }

      else if( pID==0){
      
      execve(command,tVec,envp);
      }
    }
    // After failing to get the path we will look in all the possible paths for the path with your exe
    
    else{
    
      for(int i=0 ; paths[i] != '\0';i++){
	
	char *commPath = concString(paths[i],command);// Concatenation of the path and the command

	if(stat(commPath,&buf) == 0 ){// if its found  in the path provided go ahead 
      write(1,"Command Found!\n",15);
      found=1;
      pID = fork();

      
      if(pID < 0){
      perror("Fork Error");
      }

      else if( pID==0){
      
      execve(commPath,tVec,envp);
      }
      // After we create the child we proceed to make the parent wait 
      else{

	int waitVal, waitStatus;
	char buf[100];
	int childStatus; 
	waitVal= waitpid(pID,&waitStatus,0);//Waiting parent Zzzzz...
	if (waitStatus != 0){
	  printf("Terminated with %i\n", waitStatus);
	}
	if (waitVal == pID) {
	  printf("child exited with value %d, %d\n", waitStatus, WEXITSTATUS(waitStatus));
      }
	else {
	  printf("strange: waitpid returned %d\n", waitVal);
    }
  
      }
      
	}
      }

      
      if(!stringComp(command,"exit")&& found!=1){
      write(1,"Command NOT Found!\n",19);  
    }
	}
      	
  }
  return 1;
} 
