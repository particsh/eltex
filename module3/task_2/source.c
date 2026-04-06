#include "header.h"

void command_cycle(){
    char line[1024];
    char *args[64];

    while(1){
        printf(">) ");
        fflush(stdout);
        if (fgets(line, sizeof(line), stdin)) {
            line[strcspn(line, "\n")] = '\0';
        }

        if (!separate_input(line, args, 64)){
            pid_t pid = fork();
            if (pid == 0) {
                execvp(args[0], args);
            } else if (pid > 0) {
                int status;
                waitpid(pid, &status, 0);
            } else {
                perror("fork");
            }
    }

    } 
}


// Разбор аргументов
int separate_input(char* input, char** args, int max_args) {
    int flag = 0;
    int i = 0;
    char *token = strtok(input, " \t");
    while (token && i < max_args - 1) {
        args[i++] = token;
        token = strtok(NULL, " \t");
    }
    args[i] = NULL;

    // проверка на пустую строку
    if (i == 0) flag = 1;
    return flag;
}