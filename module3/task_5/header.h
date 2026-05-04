#ifndef HEADER_H_
#define HEADER_H_

#define _GNU_SOURCE

#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

void cycle(); // основной цикл программы
int int_to_string(int increment, char* buffer); // перевод целого в строчку
void handle_signal(int sig); // обработчик сигналов

#endif