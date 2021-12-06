#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include<stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <string.h>

bool status = 0; // 1 when child is working
char path[128] = "/bin/";


void print_command_promt(){
    printf("romt:~$ ");
}

void read_and_save_command(){
    char word[100];
    scanf("%99s", &word[0]);
    strcat (path, word);
    printf("read command: %s\n\n", path);
}

int main(int argc, char **argv) {
    pid_t  pid;
    int status = 1000;

    while(1) {
        print_command_promt();
        read_and_save_command();

        pid = fork();

        if(pid > 0) {
            // parent process
            wait(NULL);
            //printf("parent active again\n");
        }
        else if(pid == 0){
            // child process
            //char* argv[] = { "/Documents", NULL };
            char* envp[] = { NULL };

            printf("command is: %s", path);
            if(execve(path, argv, envp) == -1){
                perror("execve failed");
            }

            /*//working 
            if(execve("/bin/date", argv, envp) == -1){
                perror("execve failed");
            }
            //printf("child processed\n");*/

            exit(0);
        }
        else{
            printf("Child process coudn't be created!");
        }
    }
}