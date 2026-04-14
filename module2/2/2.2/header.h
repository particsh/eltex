#ifndef HEADER_H_
#define HEADER_H_

#include <float.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

void menu();
void safe_read_string(char *buffer, int max_len, char *prompt);
double safe_read_double(char *prompt);
double add(double a, double b);
double sub(double a, double b);
double mul(double a, double b);
double div(double a, double b);

#endif