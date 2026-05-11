#ifndef HEADER_H_
#define HEADER_H_

#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_MSG 10
#define MAX_MSG_SIZE 256
#define EXIT_PRIORITY 255


void safe_read_string(char *buffer, int max_len, char *prompt); // безопасное чтение строки + сообщение
void cycle(char* name_sender, char* name_receive, int flag); // основной цикл программы

#endif