#include "header.h"

double multiplication_by_2(double number){
    return number * 2;
}

int scan_input(char *string, double * value){
    char* check_error;
    int flag = 0;
    *value = strtod(string, &check_error);
    if (*check_error != '\0' || check_error == string){
        flag = 1;
    }
    return flag;
}

int separtions_tasks(int argc, char *argv[]){
    int flag = 0;
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        flag = 1;
        return flag;
    }

    else if (pid == 0){
        // Дочерний процесс
        int split = 1 + (argc - 1) / 2;
        double value = 0;
        for (int i = split; i < argc; i++) {
            if (scan_input(argv[i], &value) == 1){
                printf("д %s\n", argv[i]);
            } else{
                printf("д %f %f\n", value, multiplication_by_2(value));
            }
            value = 0;
        }

    }

    else {
        // Родительский процесс
        int split = 1 + (argc - 1) / 2;
        double value = 0;
        for (int i = 1; i < split; i++) {
            if (scan_input(argv[i], &value) == 1){
                printf("р %s\n", argv[i]);
            } else{
                printf("р %f %f\n", value, multiplication_by_2(value));
            }
            value = 0;
        }
        wait(NULL);
    }
    return flag;
}

