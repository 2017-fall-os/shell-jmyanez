
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
  for(int j =0; s2[j] != '\0';j++,i++){
    newString[i]=s2[j]; 
  }
  newString[i] = '\0';

  return newString;
}

 
char* getPrevDir(char* cwd){
  char* prevDir = (char*)malloc(100);
  prevDir="/";
      
  char **dirs = myToc(cwd,'/');
  
    for(int i =0; dirs[i]!='\0';i++){
      if(dirs[i+1]=='\0'){
	printf("%s\n",prevDir);
	return prevDir;
      }
      
      prevDir=concString(prevDir,dirs[i]);
      prevDir=concString(prevDir,"/");
  }
  printf("%s\n",prevDir);
  
}


// Shell main functin

int main(int arc, char **argv, char **envp){
 
  char cmd1[100];
  int out = 0, pID, found=0; // verifies if te program keeps running
  char **tVec;//This will save the tokenized strings
  char **paths,  *path;
  struct stat buf;
  struct stat buf2;

  while(out!=1){

    char *command = NULL;    
    
    write(1,"$",1);
    scanf("%[^\n]%*c", cmd1);
    out = stringComp(cmd1,"exit");
    tVec= myToc(cmd1,' ');
    char *cwd = getcwd(cwd,100);
    
    command= tVec[0];
    path =getPath(envp);// This will call the getPath function and returns a string with all the paths
    paths=myToc(path,':');// This will separate all the paths possibles for the command by ':'
    
    char **tPipes = myToc(cmd1,'|');
    
      
    // If the command is change directory "cd" then we will execute the chdir() passing the dessired directory 
    if(stringComp(command,"cd")){
       if(tVec[1]!='\0' && stringComp(tVec[1],"..")){
	 char* prevDir = (char*)malloc(1024);
	 prevDir = getPrevDir(cwd);
		 	 chdir(prevDir);
	 
      	}

       else chdir(tVec[1]);
         
    }
    // If there was not a cd command then we check for any pipes, if in tPipes where we are saving our tokens for
    //pipes at [1] is not null then we will execute the next 
    if(tPipes[1]!='\0'){//If there is a pipe '|'
      char **pipeComm1 = myToc(tPipes[0],' ');
      char **pipeComm2 = myToc(tPipes[1],' ');
      printf("La Pipa de la paz\n");
      if(stat(command,&buf) == 0 ){
      write(1,"Command Found with a pipe!\n",27);
      found=1;
      int pid = fork();     
      
      if(pid < 0)
	perror("Fork Error");

      else if( pid==0){
	printf("child: fork returned %d\n", pid);
	execve(command,pipeComm1,envp);
      }//END OF elseifpid==0 

      else{
	printf("HEllo this is parmen\n");
	int waitVal, waitStatus;
	int childStatus; 
	printf("parent: child's pid=%d\n", pid);

	waitVal= waitpid(pID,&waitStatus,0);//Waiting parent Zzzzz...
	
        pid = fork(); //Child 2 adopted 
	int *pipeFds;
	if(pid < 0)
	perror("Fork Error");

	else if( pid==0){
	printf("child2: fork returned %d\n", pid);

 
	execve(command,pipeComm2,envp);
	}

	else{ //Back to parent second time 
	
	printf("HEllo this is parent2\n");
        
	printf("parent: child's pid=%d\n", pid);

	waitVal= waitpid(pID,&waitStatus,0);//Waiting parent Zzzzz...

	//pipeFds = (int *) calloc(2, sizeof(int));
	//pipe(pipeFds);

	}// Else

      }//Else
      }// IF THE PATH IS GIVE FINISHED
      //ELSE IF THE PATH IS NOT GIVEN
      else{
	printf("Path not given buddy!!!\n");
	for(int i=0 ; paths[i] != '\0';i++){
	
	char *commPath = concString(paths[i],"/");// Concatenation of the path and the command
	commPath = concString(commPath,pipeComm1[0]);
	if(stat(commPath,&buf) == 0 ){// if its found  in the path provided go ahead 
      write(1,"Command Found!\n",15);
      found=1;
      int pid1 = fork();
      if(pid1 < 0)
	perror("Fork Error");
      
      else if( pid1==0)
	execve(commPath,pipeComm1,envp);
      
      // After we create the child we proceed to make the parent wait 
      else{

	int waitVal1, waitStatus1;
	int childStatus; 
	waitVal1= waitpid(pid1,&waitStatus1,0);//Waiting parent Zzzzz...
	//SECOND PATH SEEKING
	for(int j=0 ; paths[j] != '\0';j++){
	
	char *commPath = concString(paths[j],"/");// Concatenation of the path and the command
	commPath = concString(commPath,pipeComm2[0]);
	if(stat(commPath,&buf2) == 0 ){// if its found  in the path provided go ahead 
      write(1,"Command Found!\n",15);
      found=1;
      int pid2 = fork();
      if(pid2 < 0)
	perror("Fork Error");
      
      else if( pid2==0){
	printf("Hello this is execve2\n");
	printf("Path for second command is: %s\n",commPath);
	execve(commPath,pipeComm2,envp);
      }
      // After we create the child we proceed to make the parent wait 
      else{

	int waitVal2, waitStatus2;
	waitVal2= waitpid(pid2,&waitStatus2,0);//Waiting parent Zzzzz...
	
      } //END SECOND wait parent 

	}//END IF command found in Path SECOND 
	 
	}//END FOR checking Paths SECOND
 	
      } //ENDFirst wait parent 

	}//END IF command found in Path 
	
	}//END FOR checking Paths


      }
    }


    //ORIGINAL COMMAND CHECK  
    //Check the command with stat() system call.
    //This first stat call will asume that you were given the path
    //in case of failling we will try the aproach of looking trough all the paths
    else if(stat(command,&buf) == 0 ){
      write(1,"Command Found!\n",15);
      found=1;
      pID = fork();

      
      if(pID < 0)
	perror("Fork Error");
      

      else if( tPipes[1]!="\0")
	execve(command,tVec,envp);
     
    }
	
    // After failing to get the path we will look in all the possible paths for the path with your exe
    
    else{
    
      for(int i=0 ; paths[i] != '\0';i++){
	
	char *commPath = concString(paths[i],"/");// Concatenation of the path and the command
	commPath = concString(commPath,command);
	if(stat(commPath,&buf) == 0 ){// if its found  in the path provided go ahead 
      write(1,"Command Found!\n",15);
      found=1;
      pID = fork();

      
      if(pID < 0)
	perror("Fork Error");
   

      else if( pID==0)
	execve(commPath,tVec,envp);
      
      // After we create the child we proceed to make the parent wait 
      else{

	int waitVal, waitStatus;
	char buf[100];
	int childStatus; 
	waitVal= waitpid(pID,&waitStatus,0);//Waiting parent Zzzzz...
 
      }
      }
      
	}
      }

    
      if(!stringComp(command,"exit")&& found!=1){
      write(1,"Command NOT Found!\n",19);  
      }
    	
      	
  }  
  return 1;

  
}
