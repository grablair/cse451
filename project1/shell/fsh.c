#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


#define EXIT_COMMAND "exit"
#define CHDIR_COMMAND "chdir"
#define MAX_ARGS 6

int exec_command(char* input, int old_exit_status);

int read_from_file(char* filename, int old_exit_status){
  char* line = NULL;
  size_t len = 0;
  ssize_t read;
  if(filename == NULL){
    printf("No file name given\n");
    return old_exit_status;
  }
  FILE* fd = fopen(filename, "r");
  if(fd == NULL){
    printf("File name error");
    return old_exit_status;
  }
  while((read = getline(&line, &len, fd)) != -1){ 
    if(line[0] == '\n'){
      continue;
    }else if(line[read-1] == '\n'){
      line[read-1] = '\0';
    }
    old_exit_status = exec_command(line, old_exit_status);
    
    }

  free(line);
  return old_exit_status;
}

int exec_command(char* input, int old_exit_status){
  //tokenize input
  int num_args = MAX_ARGS;  
  char** args = calloc(num_args, sizeof(char*));
    
    if(args == NULL){
      printf("Error allocating memory\n");
      return old_exit_status;
    }
    int i;
    char* arg = strtok(input, " ");
    for(i = 0; arg != NULL; i++){
      if(i >= num_args - 1){  
	num_args *= 2;
	args = realloc(args, num_args);
      }
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
      old_exit_status = read_from_file(args[1], old_exit_status);

    }else if(!strncmp(EXIT_COMMAND, command, 5)){   //exit command
      if(args[1] != NULL){         //exit with specified value
	int x = atoi(args[1]);
	free(input);
	free(args);
	printf("exit: %d\n", x);
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
	
	free(args);
	return exit_status;
      }
      
    }

    free(args);
    return old_exit_status;
}


int main(int argc, char * argv[], char * envp[]){
  int exit_stat = 0;

  while(1){
    //read line of input
    char* input = readline("CSE451Shell\% ");
    if(!strcmp(input, "")){
      free(input);               
      continue;
    }
    
    exit_stat = exec_command(input, exit_stat);
    free(input);
  }

  


  return 0;
}
