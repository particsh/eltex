#include "header.h"

void menu(){

    // заполняем память нулями
    char action[25] = {0};
    double a, b = 0;

    // создаем и заполняем реестр командами
    command_registry* commands = create_registry();
    register_command(commands, "add", add);
    register_command(commands, "sub", sub);
    register_command(commands, "mul", mul);
    register_command(commands, "div", fdiv);
    register_command(commands, "sin", sin_wrapper);
    register_command(commands, "cos", cos_wrapper);


    while(1){

        // читаем данные
        safe_read_string(action, 25, "Введите желаемое действие: \n");


        // выход из цикла
        if (!strcmp(action, "end")){
            destroy_registry(commands);
            break;
        }

        a = safe_read_double("Введите аргумент a: \n");
        if (strcmp(action, "sin") && strcmp(action, "cos")){
            b = safe_read_double("Введите аргумент b: \n");
        }
        

        // выбор функции
        operation_func operation = find_command(commands, action);
        if (operation){
          printf("Результат: %f\n", operation(a, b));
        } else{
          printf("Неизвестная комманда: %s\n", action);
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
double fdiv(double a, double b) { return a / b; }

// так как синус и косинус унарные, заглушки, дабы работало с нашей структурой
double sin_wrapper(double a, double b) { (void)b; return sin(a); }
double cos_wrapper(double a, double b) { (void)b; return cos(a); }


// регистрация команды
int register_command(command_registry *target, const char *name, operation_func func){

    // проверка на дубликат
    for (size_t i = 0; i < target->count; i ++){
        if (!strcmp(target->entries[i].name, name)){
            printf("Ошибка: такая функция уже есть в списке");
            return -1;
        }
    }

    // Расширение, если нет места
    if (target->count == target->capacity){
        size_t new_capacity = target->capacity * 2;
        command *new_entries = realloc(target->entries, new_capacity * sizeof(command));
        if (!new_entries) return -1; // нехватка памяти
        target->entries = new_entries;
        target->capacity = new_capacity;
    }

    // копирование имени + выделение памяти
    char* name_copy = malloc(strlen(name) + 1);
    if (!name_copy) return -1;
    strcpy(name_copy, name);

    // новая запись
    target->entries[target->count].name = name_copy;
    target->entries[target->count].func = func;
    target->count++;
    return 0;


}

// удаление команды
int unregister_command(command_registry *target, const char *name){
  for (size_t i = 0; i < target->count; i++){
    if (strcmp(target->entries[i].name, name) == 0){
      free((void*)target->entries[i].name); // освобождаем копию имени, приведение к войду, т.к. там конст стоит
      // Сдвигаем хвост массива влево
      memmove(&target->entries[i], &target->entries[i+1], (target->count - i - 1) * sizeof(command));
      target->count--;
      return 0;
    }
  }
  return -1; // не найдено записи

}

// поиск команды в реестре
operation_func find_command(command_registry *target, const char *name){
  for (size_t i = 0; i < target->count;i++){
    if (strcmp(target->entries[i].name, name) == 0){
      return target->entries[i].func;
    }

  }
  return NULL;
}


// уничтожение реестра
void destroy_registry(command_registry *reg) {
    if (!reg) return;
    for (size_t i = 0; i < reg->count; i++) {
        free((void*)reg->entries[i].name);
    }
    free(reg->entries);
    free(reg);
}

// создание реестра
command_registry* create_registry(void){
  command_registry* target = malloc(sizeof(command_registry));
  if (!target) return NULL;

  target->capacity = 6;
  target->count = 0;
  target->entries = malloc(target->capacity * sizeof(command));
  if (!target->entries){
    free(target);
    return NULL;
  }
  return target;
}
