#ifndef HEADER_H_
#define HEADER_H_

#include <stdio.h>
#include <string.h>

#define MAX_CONTACTS 10
#define MAX_NAME_LEN 20
#define MAX_WORK_LEN 30
#define MAX_EMAIL_LEN 60
#define MAX_PHONE_LEN 15
#define MAX_LINK_LEN 100
#define MAX_EMAILS 10
#define MAX_PHONES 10
#define MAX_LINKS 10

typedef struct person {
  char FirstName[MAX_NAME_LEN];
  char LastName[MAX_NAME_LEN];
  char Patronymic[MAX_NAME_LEN];
  char PlaceOfWork[MAX_WORK_LEN];
  char Emails[MAX_EMAILS][MAX_EMAIL_LEN];
  char NumbersOfPhone[MAX_PHONES][MAX_PHONE_LEN];
  char LinksSocialNetwork[MAX_LINKS][MAX_LINK_LEN];
} Person;

void menu(Person *target, int size);

int del_person(Person *target, int size);
int add_person(Person *target, int size);
int edit_person(Person *target, int size);
int check_person(Person *target, int size);
int check_actual_size(Person *target, int size);
int find_first_free(Person *target, int size);
int safe_read_int(char *prompt);
void safe_read_string(char *buffer, int max_len, char *prompt);

#endif
