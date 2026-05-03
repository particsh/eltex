#ifndef HEADER_H_
#define HEADER_H_

#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATH_MAX 4096
#define MAX_ARGS 64

void command_cycle();

// структура под одну комманду
typedef struct Command{
    char* argv[MAX_ARGS]; // команда и ее аргументы
    char input_file[PATH_MAX]; // имя файла для <
    char output_file[PATH_MAX]; // имя файла для > и >>
    int append; // 0 для >, 1 для >>
}Command;

// структура для пайплайна
typedef struct Pipeline{
    Command *commands; // команды
    int ncommands; // количество команд
    int max_ncommands; // максимальное количество команд
}Pipeline;

int separate_command(char* input, Command *cmd); // разбираем комманду
int separate_pipeline(char* input, Pipeline *pl); // разбираем пайплайн



#endif