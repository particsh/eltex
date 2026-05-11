#ifndef HEADER_H_
#define HEADER_H_

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define MAX_NUM 150
#define MAX_LEN_OF_NUMS 50
#define MAX_LINE_SIZE 512


void processing_line(const char* line, int *max, int *min);
int generate_random_line(char **line);
int num_of_tens(int num);
void producer_cycle(char* filename);
void consumer_cycle(char* filename);




#endif