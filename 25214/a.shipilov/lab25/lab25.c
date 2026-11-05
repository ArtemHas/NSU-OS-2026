#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>

#define BUFFER_SIZE 256

int main(void){
    int pipefd[2];
    pid_t pid; 

    if(pipe(pipefd) == -1){
        perror("Ошибка pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if(pid < 0){
        perror("Ошибка fork");
        exit(EXIT_FAILURE);
    }
    if(pid > 0){
        const char *message = "Hello from parent process\n";

        close(pipefd[0]);

        if(write(pipefd[1], message, strlen(message)) == -1){
            perror("Ошибка write");
            close(pipefd[1]);
            exit(EXIT_FAILURE);
        }

        close(pipefd[1]);
        if(wait(NULL) == -1) {
            perror("Ошибка wait");
            exit(EXIT_FAILURE);
        }
    }else{
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;

        close(pipefd[1]);
        while((bytes_read = read(pipefd[0], buffer, sizeof(buffer)-1)) > 0){
            for(ssize_t i = 0; i < bytes_read; i++){
                buffer[i] = (char)toupper((unsigned char)buffer[i]);
            }
            buffer[bytes_read] = '\0';
            printf("%s", buffer);
        }

        if (bytes_read == -1) {
            perror("read");
            close(pipefd[0]);
            exit(EXIT_FAILURE);
        }

        close(pipefd[0]);

    }
    return 0;
}

