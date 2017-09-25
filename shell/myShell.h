#ifndef myShellIncluded
#define myShellIncluded



char* getPath(char **envp);

int countChar(char* aString);

int stringComp(char* cmd1, char* cmd2);

char* concString(char* s1, char* s2);

int main(int arc, char **argv, char **envp);

#endif

