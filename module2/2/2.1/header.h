#ifndef HEADER_H_
#define HEADER_H_
#include <stdio.h>
#include <string.h>

typedef struct person
{
    char FirstName[20];
    char LastName[20];
    char Patronymic[20];
    char PlaceOfWork[30];
    char Emails[10][60];
    char NumbersOfPhone[10][15];
    char LinksSocialNetwork[10][100];
} Person;

void menu(Person* target, int size);

int del_person(Person* target, int size);
int add_person(Person* target, int size);
int edit_person(Person* target, int size);
int check_person(Person* target, int size);
int check_actual_size(Person* target, int size)


#endif
