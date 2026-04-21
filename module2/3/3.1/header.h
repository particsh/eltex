#ifndef HEADER_H_
#define HEADER_H_

#include <stdio.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

void check_format_byte(unsigned int input);
void check_format_char(unsigned int input);
void check_format_int(unsigned int input);


#endif