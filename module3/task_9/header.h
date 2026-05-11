#ifndef HEADER_H_
#define HEADER_H_

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>

#define MAX_NUM 150
#define MAX_LEN_OF_NUMS 50
#define MAX_LINE_SIZE 512
#define NAME_SEM "/file_semaphore"


void processing_line(const char* line, int *max, int *min);
int generate_random_line(char **line);
void cycle(char* filename);

#endif