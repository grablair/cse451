#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


#define EXIT_COMMAND "exit"
#define CHDIR_COMMAND "chdir"

int exec_command(char* input, int old_exit_status){
  //tokenize input
  //char* command = strtok(input, " ");
    char** args = calloc(20, sizeof(char*));
    int i;
    char* arg = strtok(input, " ");
    for(i = 0; arg != NULL; i++){
      args[i] = arg;
      arg = strtok(NULL, " ");
    }
    char* command = args[0];
    
    //chdir command
    if(!strncmp(CHDIR_COMMAND, command, 6)){
      int ret;
      if(args[1] != NULL){    //use first arg as dir
	ret = chdir(args[1]);
      }else{                  //cd to home
	char* home = getenv("HOME");
	printf("%s\n", home);
	ret = chdir(home);
      }
      if(ret != 0){
	printf("Directory does not exist\n");
      }
    }else if(!strncmp(".", command, 2)){
      //read from file
      

    }else if(!strncmp(EXIT_COMMAND, command, 5)){   //exit command
      if(args[1] != NULL){         //exit with specified value
	int x = atoi(args[1]);
	free(input);
	free(args);
	exit(x);                //CHANGE TO exit(x)
      }
      free(input);
      free(args);  
      exit(old_exit_status);                    
    }else{                     //execute command
      pid_t pid = fork();
      if(pid == 0){
	int exe = execvp(command, args);
	if(exe == -1){
	  printf("Invalid Command\n");
	}
	exit(0);
      }else{
	int status;
	int exit_status = -1;
	if(wait(&status) > 0){
	  //need to check if exited normally??
	  exit_status = WEXITSTATUS(status);
	
	}
	free(input);
	free(args);
	return exit_status;
      }
      
    }

    free(input);
    free(args);
    return old_exit_status;
}


int main(int argc, char * argv[], char * envp[]){
  int exit_stat = 0;

  while(1){
    //read line of input
    char* input = readline("CSE451Shell\% ");
    if(!strcmp(input, "")){
      free(input);               //SEGFAULT on no input
      continue;
    }
    
    exit_stat = exec_command(input, exit_stat);
  }

  


  return 0;
}
