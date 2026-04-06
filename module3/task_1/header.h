#ifndef HEADER_H_
#define HEADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

double multiplication_by_2(double number);

int scan_input(char *string, double * value);

int separtions_tasks(int argc, char *argv[]);



#endif