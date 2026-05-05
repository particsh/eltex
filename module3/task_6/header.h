#ifndef HEADER_H_
#define HEADER_H_


// 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct msgbuf {
    long mtype;
    char mtext[128];
}msgbuf;

typedef struct clients{
    long numbers_clients[100];
    int count;
}clients;

void cycle_server();

void cycle_client(int mtype);

void safe_read_string(char *buffer, int max_len, char *prompt); // безопасное чтение строки + сообщение




#endif