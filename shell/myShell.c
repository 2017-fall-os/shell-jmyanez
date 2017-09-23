#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int stringComp(char* cmd1, char* cmd2){
  for(int i=0;cmd1[i]!='\0' || cmd2[i]!='\0' ;i++){
    if(cmd1[i] != cmd2[i]){ 
      return 0;
    }//If statx
  }//For loop
  
  return 1;
 

}

int main(){
 
   char cmd1[30];
   int flag =0;
   
  while(flag!=1){
    write(1,"$",1);
     scanf("%[^\n]%*c", cmd1);
    flag = stringComp(cmd1,"exit");
	
  }
  return 1;
}
