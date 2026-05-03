#include "header.h"

void command_cycle(){

    // объявляепм буффер
    char line[1024];
    char *args[64];

    while(1){
        // привественое сообщение
        printf(">) ");

        // очистка буффера после printf
        fflush(stdout);
        // считывание ввода
        if (fgets(line, sizeof(line), stdin)) {
            // добавление в конец нуль символа
            line[strcspn(line, "\n")] = '\0';
        }

        // Разбор аргументов
        if (!separate_input(line, args, 64)){
            // создаем процесс
            pid_t pid = fork();
            if (pid == 0) {
                // запускаем в дочернем процессе программу
                execvp(args[0], args);
            } else if (pid > 0) {
                int status;
                // ждем дочернгий процесс
                waitpid(pid, &status, 0);
            } else {
                // ошибка создания процесса
                perror("fork");
            }
    }

    } 
}


// Разбор аргументов
int separate_input(char* input, char** args, int max_args) {
    int flag = 0;
    int i = 0;
    // режем строку на части по пробелу и табуляции
    char *token = strtok(input, " \t");
    while (token && i < max_args - 1) {
        args[i++] = token;
        // продолжаем
        token = strtok(NULL, " \t");
    }
    // добавляем в конец NULL дабы exec не жаловолся
    args[i] = NULL;

    // проверка на пустую строку
    if (i == 0) flag = 1;
    return flag;
}