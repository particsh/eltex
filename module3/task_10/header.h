#ifndef HEADER_H_
#define HEADER_H_

#define _POSIX_C_SOURCE 200809L

#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_NUM 150
#define MAX_LEN_OF_NUMS 50
#define MAX_LINE_SIZE 512

#define DATA_READY 0
#define RESULT_READY 1

typedef struct SharedData
{
    char line[MAX_LINE_SIZE];
    int max;
    int min;
    int stop_flag;
}SharedData;

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};
 

void processing_line(const char* line, int *max, int *min);
void sem_wait(int semid, int sem_num);
void sem_signal(int semid, int sem_num);
void remove_semaphores(int semid);
int generate_random_line(char *line, size_t size);
void cycle();

#endif