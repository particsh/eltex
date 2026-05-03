#ifndef HEADER_H_
#define HEADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>



// умножение на два 
double multiplication_by_2(double number);


// обработка ввода
int scan_input(char *string, double *value);


// разделение задач
int separtions_tasks(int argc, char *argv[]);



#endif