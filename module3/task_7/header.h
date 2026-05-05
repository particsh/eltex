#ifndef HEADER_H_
#define HEADER_H_

#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>


void safe_read_string(char *buffer, int max_len, char *prompt); // безопасное чтение строки + сообщение

#endif