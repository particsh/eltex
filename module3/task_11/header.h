#ifndef HEADER_H_
#define HEADER_H_

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>
#include <errno.h>

#define MAX_NUM 150
#define MAX_LEN_OF_NUMS 50
#define MAX_LINE_SIZE 512

#define NAME_SHM "/shared_memory"

typedef struct SharedData
{
    char line[MAX_LINE_SIZE];
    int max;
    int min;
    int stop_flag;

    sem_t data_ready;
    sem_t result_ready;
}SharedData;

void processing_line(const char* line, int *max, int *min);
int generate_random_line(char *line, size_t size);
void handle_signal(int sig);
void cycle();


#endif