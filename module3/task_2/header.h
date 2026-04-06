#ifndef HEADER_H_
#define HEADER_H_

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

void command_cycle();

int separate_input(char* input, char** args, int max_args);

#endif