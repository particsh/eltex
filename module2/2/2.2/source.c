#include "header.h"

void menu(){

    // заполняем память нулями
    char action[25] = {0};
    double a, b = 0;

    while(1){

        // читаем данные
        safe_read_string(action, 25, "Введите желаемое действие: \n");
        a = safe_read_double("Введите аргумент a: \n");
        if (strcmp(action, "sin") && strcmp(action, "cos")){
            b = safe_read_double("Введите аргумент b: \n");
        }
        

        // выход из цикла
        if (!strcmp(action, "end")){
            break;
        }

        // выбор функции 
        if (!strcmp(action, "add")){
            printf("Результат: %f\n", add(a, b));
        } else if(!strcmp(action, "sub")){
            printf("Результат: %f\n", sub(a, b));
        } else if(!strcmp(action, "mul")){
            printf("Результат: %f\n", mul(a, b));
        } else if(!strcmp(action, "div")){
            printf("Результат: %f\n", div(a, b));
        }  else if(!strcmp(action, "sin")){
            printf("Результат: %f\n", sin(a));
        }  else if(!strcmp(action, "cos")){
            printf("Результат: %f\n", cos(b));
        } else{
            printf("Функция не найдена\n");
        }




    }
}

// Безопасное чтение действительного числа с огранинной длиной + сообщение

double safe_read_double(char *prompt) {
  double value;
  while (1) {
    printf("%s", prompt);
    if (scanf("%lf", &value) == 1) {
      while (getchar() != '\n'); // очистка буффера
      return value;
    }
    printf("Ошибка ввода. Введите действительное число.\n");
    while (getchar() != '\n'); // очистка буффера
  }
}

// Безопасное чтение строки с огранинной длиной + сообщение

void safe_read_string(char *buffer, int max_len, char *prompt) {
  printf("%s", prompt);
  if (fgets(buffer, max_len, stdin) == NULL) {
    buffer[0] = '\0'; // вставка нуль символа в случае ошибки
    return;
  }
  size_t len = strlen(buffer);
  if (len > 0 && buffer[len - 1] == '\n') // проверка на пустоту
    buffer[len - 1] = '\0'; // вставка нуль символа в конец
  else {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF); // очистка буффера
  }
}

// Функции операций
double add(double a, double b) { return a + b; }
double sub(double a, double b) { return a - b; }
double mul(double a, double b) { return a * b; }
double div(double a, double b) { return a / b; }
