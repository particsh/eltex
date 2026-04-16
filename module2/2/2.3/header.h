#ifndef HEADER_H_
#define HEADER_H_

#include <float.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

typedef double (*operation_func)(double, double); // указатель на функцию

typedef struct command{
    const char *name; // имя функции
    operation_func func; // функция
}command;

typedef struct {
    command *entries;   // динамический массив записей
    size_t count;       // текущее количество
    size_t capacity;    // выделенная память (для массива)
} command_registry;


command_registry* create_registry(void);
int register_command(command_registry *target, const char *name, operation_func func); // регистрация команды
int unregister_command(command_registry *target, const char *name); // удаление команды
operation_func find_command(command_registry *target, const char *name); // поиск команды в реестре
void destroy_registry(command_registry *reg); // уничтожение реестра

void menu(); // менюшка
void safe_read_string(char *buffer, int max_len, char *prompt); // безопасное чтение строки + сообщение
double safe_read_double(char *prompt); // безопасное чтение действительного числа + сообщение

// функции операций
double add(double a, double b);
double sub(double a, double b);
double mul(double a, double b);
double fdiv(double a, double b);

// так как синус и косинус унарные, заглушки, дабы работало с нашей структурой
double sin_wrapper(double a, double b);
double cos_wrapper(double a, double b);

#endif