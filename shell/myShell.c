
/*
Code created by: Jose Yanez
Finishd: 10/01/2017-:-9:51 PM MT
 */
#include <stdio.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytoc.c"
#include "myShell.h"

char** eliminateLastTok(char **tVec){
  char** newVec;

  for(int i=0;tVec[i+1]!='\0';i++){
    newVec[i]=tVec[i];
    //  printf("%s\n", tVec[i]);
  }

  return newVec;
}

int tokenCounter(char **tVec){
  int i;
  for( i=0; tVec[i] != '\0';i++){
  
  }
  return i;
}

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
	//	printf("%s\n",prevDir);
	return prevDir;
      }
      
      prevDir=concString(prevDir,dirs[i]);
      prevDir=concString(prevDir,"/");
  }
    //  printf("%s\n",prevDir);
  
}


// Shell main functin

int main(int arc, char **argv, char **envp){
 
  char cmd1 [1024];
  cmd1[0]='\0';
  int out = 0, pID, found=0; // verifies if te program keeps running
  char **tVec;//This will save the tokenized strings
  char **paths,  *path;
  struct stat buf;
  struct stat buf2;
 
  while(out!=1){
   
    char *command = NULL;    
    
char foo =  scanf(" %[^\n]%*c",cmd1);
// printf("Buffer is:%c\n",cmd1);
    
    out = stringComp(cmd1,"exit");
 
        if(foo == EOF)exit(0);//Exit if EOF

    out = stringComp(cmd1,"exit");

    tVec= myToc(cmd1,' ');
    char *cwd = getcwd(cwd,100);
    
    command= tVec[0];
    path =getPath(envp);// This will call the getPath function and returns a string with all the paths
    paths=myToc(path,':');// This will separate all the paths possibles for the command by ':'

    
    char **tPipes = myToc(cmd1,'|');
    int tokensCounted = tokenCounter(tVec);
      if(cmd1[0]=='0'){
	cmd1[0]='0';
	out=0;
	scanf("%[^\n]%*c",cmd1);
      }

      //NOW WE WILL CHECK IF THE USER WANTS THIS PROCESS IN THE BACKGROUND
      if(stringComp(tVec[tokensCounted-1],"&")){
	
	char** tVec2 = myToc(cmd1,'&');
	char** bgdToks =myToc(tVec[0],' ');
       
	 if(stat(bgdToks[0],&buf) == 0 ){
	  
      found=1;
      pID = fork();

      
      if(pID < 0)
	perror("Fork Error");
      

     
      else if(pID==0)
	execve(bgdToks[0],tVec2,envp);

      else {
	int waitVal, waitStatus;
	char buf[100];
	int childStatus; 
		waitVal= waitpid(-1,&waitStatus,WNOHANG);
	return 1;

      }
	 }
	
    // After failing to get the path we will look in all the possible paths for the path with your exe
    
    else{
    
      for(int i=0 ; paths[i] != '\0';i++){
	
	char *commPath = concString(paths[i],"/");// Concatenation of the path and the command
	commPath = concString(commPath,bgdToks[0]);
	if(stat(commPath,&buf) == 0 ){// if its found  in the path provided go ahead 
	  found=1;
	  pID = fork();

      
      if(pID < 0)
	perror("Fork Error");
   

      else if( pID==0)
	execve(bgdToks[0],tVec2,envp);
     
      
      // After we create the child we proceed to make the parent wait 
      else{

	int waitVal, waitStatus;
	char buf[100];
	int childStatus; 
	waitVal= waitpid(-1,&waitStatus,WNOHANG); //WNOHANG will make waitpid return 0; 
      }
      }
      
      }
    }
      }
  
    // If the command is change directory "cd" then we will execute the chdir() passing the dessired directory 
      else  if(stringComp(command,"cd")){
       if(tVec[1]!='\0' && stringComp(tVec[1],"..")){
	 char* prevDir = (char*)malloc(1024);
	 prevDir = getPrevDir(cwd);
		 	 chdir(prevDir);
	 
      	}

       else chdir(tVec[1]);
         
    }
    // If there was not a cd command then we check for any pipes, if in tPipes where we are saving our tokens for
    //pipes at [1] is not null then we will execute the next 
      else if(tPipes[1]!='\0'){//If there is a pipe '|'
      int fd[2]; //file descriptor
      char **pipeComm1 = myToc(tPipes[0],' ');
      char **pipeComm2 = myToc(tPipes[1],' ');
      
      if(stat(command,&buf) == 0 ){
	//write(1,"Command Found with a pipe!\n",27);
      found=1;
      pipe(fd); // System Call pipe !!!
      int pid = fork();     
      
      if(pid < 0)
	perror("Fork Error");

      else if( pid==0){
-        //printf("pipeComm1:\n", pipeComm1[1]);
	dup2(fd[1],1);//Substitute '1' with fd[1] 
	close(fd[0]);//Close input 
	execve(pipeComm1[0],pipeComm1,envp);
	//execve(tPipes[0],pipeComm1,envp);
     
      }//END OF elseifpid==0 

      else{
	//printf("HEllo this is parent\n");
	int waitVal, waitStatus;
	int childStatus; 

	close(fd[1]);
	waitVal= waitpid(pID,&waitStatus,0);//Waiting parent Zzzzz...
        pid = fork(); //Child 2 adopted 
	
	if(pid < 0)
	perror("Fork Error");

	else if( pid==0){
	  dup2(fd[0],0);
	  close(fd[1]);
	    execve(pipeComm2[0],pipeComm2,envp);
	  //   execve(tPipes[1],pipeComm2,envp);
	  	}

	else{ //Back to parent second time 

	dup2(1,fd[1]);
	dup2(0,fd[0]);
	waitVal= waitpid(pid,&waitStatus,0);//Waiting parent Zzzzz...
	}// ElseEND

      }//ElseEND
      }// IF THE PATH GIVEN FINISHED
      //ELSE IF THE PATH IS NOT GIVEN
      else{
	int fd[2]; //file descriptor
	for(int i=0 ; paths[i] != '\0';i++){
	
	char *commPath = concString(paths[i],"/");// Concatenation of the path and the command
	commPath = concString(commPath,pipeComm1[0]);
	if(stat(commPath,&buf) == 0 ){// if its found  in the path provided go ahead 
	  //write(1,"Command Found!\n",15);
      found=1;
      pipe(fd);
      int pid1 = fork();
      if(pid1 < 0)
	perror("Fork Error");
      
      else if( pid1==0){
	dup2(fd[1],1);
	close(fd[0]);
	execve(commPath,pipeComm1,envp);
      }
      // After we create the child we proceed to make the parent wait 
      else{
	int waitVal1, waitStatus1;
	int childStatus; 
	close(fd[1]);
	waitVal1= waitpid(pid1,&waitStatus1,0);//Waiting parent Zzzzz...
	//SECOND PATH SEEKING
	for(int j=0 ; paths[j] != '\0';j++){
	
	char *commPath = concString(paths[j],"/");// Concatenation of the path and the command
	commPath = concString(commPath,pipeComm2[0]);
	if(stat(commPath,&buf2) == 0 ){// if its found  in the path provided go ahead 
	  //write(1,"Command Found!\n",15);
      found=1;
      int pid2 = fork();
      if(pid2 < 0)
	perror("Fork Error");
      
      else if( pid2==0){
	dup2(fd[0],0);
	close(fd[1]);
	execve(commPath,pipeComm2,envp);
	  
      }
      // After we create the child we proceed to make the parent wait 
      else{

	int waitVal2, waitStatus2;
	dup2(1,fd[1]);
	dup2(0,fd[0]);
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
      
      found=1;
      pID = fork();
      if(pID < 0)
	perror("Fork Error");
      else if(pID==0)
      execve(command,tVec,envp);
	
    }
	
    // After failing to get the path we will look in all the possible paths for the path with your exe
    
    else{
      char *commPath;
      for(int i=0 ; paths[i] != '\0';i++){	
	 commPath = concString(paths[i],"/");// Concatenation of the path and the command
	commPath = concString(commPath,command);

	  
      
      
        // if its found  in the path provided go ahead 
      if(stat(commPath,&buf) == 0 ){
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
      //rintf("HEY\n");
    }//END OF MASTER WHILE
        
  return 1;

  
}

