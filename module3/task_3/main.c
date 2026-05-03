#include "header.h"

int main() {
  Person people[10] = {0};
  read_file(people, 10);
  menu(people, 10);
  write_file(people, 10);
  return 0;
}