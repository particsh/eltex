#include "header.h"

// менюшка

void menu(Person *target, int size) {
  int flag = 1;
  int choice = 0;
  while (flag) {
    printf("*Книга контактов*\nВыберите опцию\n1. Добавить контакт\n2. Редактировать контакт\n3. "
           "Удалить контакт\n4. Посмотреть контакт\n5. Выход из меню\n");
    choice = safe_read_int("Выберите опцию: ");
    switch (choice) {
    case 1:
      add_person(target, size);
      break;
    case 2:
      edit_person(target, size);
      break;
    case 3:
      del_person(target, size);
      break;
    case 4:
      check_person(target, size);
      break;
    case 5:
      flag = 0;
      break;
    default:
      printf("Неверный выбор\n");
    }
  }
}

// удаление человека из списка

int del_person(Person *target, int size) {

  // проверка указателя

  if (target == NULL) {
    printf("Ошибка: указатель ведет на NULL\n");
    return 1;
  }

  int actual_size = check_actual_size(target, size); // текущий размер

  // проверка на пустоту
  if (actual_size == 0) {
    printf("Список контактов пуст, удалять нечего.\n");
    return 1;
  }

  // вывод доступных контактов
  for (int i = 0; i < actual_size; i++) {
    printf("%d. %s %s\n", i + 1, target[i].FirstName, target[i].LastName);
  }

  int choice = safe_read_int("Выберите по номеру цель для удаления: ");

  // проверка выбора на выход за пределы
  if (choice < 1 || choice > actual_size) {
    printf("Неверный номер контакта.\n");
    return 1;
  }

  int idx = choice - 1; // смещение на единицу
  memset(&target[idx], 0, sizeof(Person)); // зануление памяти по указателю

  printf("Контакт успешно удалён.\n");
  return 0;
}


// добавление человека из списка

int add_person(Person *target, int size) {

  // проверка указателя
  
  if (target == NULL) {
    printf("Ошибка: указатель ведет на NULL\n");
    return 1;
  }

  // первая свободная ячейка
  int position = find_first_free(target, size);

  // книга полностью заполнена
  if (position == -1) {
    printf("Ошибка: список контактов полностью заполнен\n");
    return 1;
  }

  safe_read_string(target[position].FirstName, MAX_NAME_LEN,
                   "Введите имя контакта: ");
  safe_read_string(target[position].LastName, MAX_NAME_LEN,
                   "Введите фамилию контакта: ");

  char choice;
  printf("Вы хотите дальше заполнять поля контакта? (y/n): ");
  scanf(" %c", &choice);
  while (getchar() != '\n'); // очистка буффера

  if (choice == 'y' || choice == 'Y') {
    safe_read_string(target[position].Patronymic, MAX_NAME_LEN,
                     "Введите отчество контакта: ");
    safe_read_string(target[position].PlaceOfWork, MAX_WORK_LEN,
                     "Введите место работы контакта: ");

    int flag = 1;
    printf("Введите почты контакта (до %d), пустая строка - закончить\n",
           MAX_EMAILS);
    for (int i = 0; i < MAX_EMAILS && flag; i++) {
      char tmp[MAX_EMAIL_LEN];
      printf("Email %d: ", i + 1);
      if (fgets(tmp, sizeof(tmp), stdin) == NULL)
        break;
      size_t len = strlen(tmp);
      if (len > 0 && tmp[len - 1] == '\n')
        tmp[len - 1] = '\0';
      else {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
      }
      if (tmp[0] == '\0') {
        flag = 0;
      } else {
        strcpy(target[position].Emails[i], tmp);
      }
    }

    printf(
        "Введите номера телефона контакта (до %d), пустая строка - закончить\n",
        MAX_PHONES);
    flag = 1;
    for (int i = 0; i < MAX_PHONES && flag; i++) {
      char tmp[MAX_PHONE_LEN];
      printf("Телефон %d: ", i + 1);
      if (fgets(tmp, sizeof(tmp), stdin) == NULL)
        break;
      size_t len = strlen(tmp);
      if (len > 0 && tmp[len - 1] == '\n')
        tmp[len - 1] = '\0';
      else {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF)
          ;
      }
      if (tmp[0] == '\0') {
        flag = 0;
      } else {
        strcpy(target[position].NumbersOfPhone[i], tmp);
      }
    }

    printf("Введите ссылки на соцсети контакта (до %d), пустая строка - "
           "закончить\n",
           MAX_LINKS);
    flag = 1;
    for (int i = 0; i < MAX_LINKS && flag; i++) {
      char tmp[MAX_LINK_LEN];
      printf("Соцсеть %d: ", i + 1);
      if (fgets(tmp, sizeof(tmp), stdin) == NULL)
        break;
      size_t len = strlen(tmp);
      if (len > 0 && tmp[len - 1] == '\n')
        tmp[len - 1] = '\0';
      else {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF)
          ;
      }
      if (tmp[0] == '\0') {
        flag = 0;
      } else {
        strcpy(target[position].LinksSocialNetwork[i], tmp);
      }
    }
  }
  return 0;
}

// редактор человека из списка

int edit_person(Person *target, int size) {

  // проверка указателя
  
  if (target == NULL) {
    printf("Ошибка: указатель ведет на NULL\n");
    return 1;
  }

  // проверка на пустоту
  int actual_size = check_actual_size(target, size);
  if (actual_size == 0) {
    printf("Список контактов пуст.\n");
    return 1;
  }

  for (int i = 0; i < actual_size; i++) {
    printf("%d. %s %s\n", i + 1, target[i].FirstName, target[i].LastName);
  }

  int choice = safe_read_int("Выберите по номеру цель для редактирования: ");
  if (choice < 1 || choice > actual_size) {
    printf("Неверный номер контакта.\n");
    return 1;
  }
  int idx = choice - 1;

  printf("1. Имя\n2. Фамилия\n3. Отчество\n4. Место работы\n5. Адреса "
         "электронной почты\n6. Номера телефонов\n7. Ссылки на соцсети\n");
  choice = safe_read_int("Выберите по номеру поле для редактирования: ");
  switch (choice) {
  case 1:
    safe_read_string(target[idx].FirstName, MAX_NAME_LEN,
                     "Введите новое значение для имени: ");
    break;
  case 2:
    safe_read_string(target[idx].LastName, MAX_NAME_LEN,
                     "Введите новое значение для фамилии: ");
    break;
  case 3:
    safe_read_string(target[idx].Patronymic, MAX_NAME_LEN,
                     "Введите новое значение для отчества: ");
    break;
  case 4:
    safe_read_string(target[idx].PlaceOfWork, MAX_WORK_LEN,
                     "Введите новое значение для места работы: ");
    break;
  case 5: {
    int email_count = 0;
    while (email_count < MAX_EMAILS &&
           target[idx].Emails[email_count][0] != '\0')
      email_count++;
    if (email_count == 0) {
      printf("Нет адресов электронной почты для редактирования.\n");
      break;
    }
    printf("Адреса электронной почты:\n");
    for (int i = 0; i < email_count; i++)
      printf("%d. %s\n", i + 1, target[idx].Emails[i]);

    int email_choice =
        safe_read_int("Выберите номер поля для редактирования: ");
    if (email_choice < 1 || email_choice > email_count) {
      printf("Неверный номер.\n");
      break;
    }
    safe_read_string(target[idx].Emails[email_choice - 1], MAX_EMAIL_LEN,
                     "Введите новое значение для адреса электронной почты: ");
    break;
  }
  case 6: {
    int phone_count = 0;
    while (phone_count < MAX_PHONES &&
           target[idx].NumbersOfPhone[phone_count][0] != '\0')
      phone_count++;
    if (phone_count == 0) {
      printf("Нет номеров телефонов для редактирования.\n");
      break;
    }
    printf("Номера телефонов:\n");
    for (int i = 0; i < phone_count; i++)
      printf("%d. %s\n", i + 1, target[idx].NumbersOfPhone[i]);

    int phone_choice =
        safe_read_int("Выберите номер поля для редактирования: ");
    if (phone_choice < 1 || phone_choice > phone_count) {
      printf("Неверный номер.\n");
      break;
    }
    safe_read_string(target[idx].NumbersOfPhone[phone_choice - 1],
                     MAX_PHONE_LEN, "Введите новое значение для телефона: ");
    break;
  }
  case 7: {
    int link_count = 0;
    while (link_count < MAX_LINKS &&
           target[idx].LinksSocialNetwork[link_count][0] != '\0')
      link_count++;
    if (link_count == 0) {
      printf("Нет ссылок на соцсети для редактирования.\n");
      break;
    }
    printf("Ссылки на соцсети:\n");
    for (int i = 0; i < link_count; i++)
      printf("%d. %s\n", i + 1, target[idx].LinksSocialNetwork[i]);

    int link_choice = safe_read_int("Выберите номер поля для редактирования: ");
    if (link_choice < 1 || link_choice > link_count) {
      printf("Неверный номер.\n");
      break;
    }
    safe_read_string(target[idx].LinksSocialNetwork[link_choice - 1],
                     MAX_LINK_LEN,
                     "Введите новое значение для ссылки на соцсеть: ");
    break;
  }
  default:
    printf("Неверный выбор\n");
    return 1;
  }
  return 0;
}

// просмотр человека из списка

int check_person(Person *target, int size) {

  // проверка указателя
  
  if (target == NULL) {
    printf("Ошибка: указатель ведет на NULL\n");
    return 1;
  }
  int actual_size = check_actual_size(target, size);
  if (actual_size == 0) {
    printf("Список контактов пуст.\n");
    return 0;
  }

  for (int i = 0; i < actual_size; i++) {
    printf("%d. %s %s\n", i + 1, target[i].FirstName, target[i].LastName);
  }

  int choice = safe_read_int("Выберите по номеру цель для просмотра: ");
  if (choice < 1 || choice > actual_size) {
    printf("Неверный номер контакта.\n");
    return 1;
  }
  int idx = choice - 1;

  printf("Имя: %s\n", target[idx].FirstName);
  printf("Фамилия: %s\n", target[idx].LastName);
  if (target[idx].Patronymic[0] != '\0')
    printf("Отчество: %s\n", target[idx].Patronymic);
  if (target[idx].PlaceOfWork[0] != '\0')
    printf("Место работы: %s\n", target[idx].PlaceOfWork);

  printf("Адреса электронной почты:\n");
  for (int i = 0; i < MAX_EMAILS; i++) {
    if (target[idx].Emails[i][0] == '\0')
      break;
    printf("%d. %s\n", i + 1, target[idx].Emails[i]);
  }

  printf("Номера телефонов:\n");
  for (int i = 0; i < MAX_PHONES; i++) {
    if (target[idx].NumbersOfPhone[i][0] == '\0')
      break;
    printf("%d. %s\n", i + 1, target[idx].NumbersOfPhone[i]);
  }

  printf("Ссылки на соцсети:\n");
  for (int i = 0; i < MAX_LINKS; i++) {
    if (target[idx].LinksSocialNetwork[i][0] == '\0')
      break;
    printf("%d. %s\n", i + 1, target[idx].LinksSocialNetwork[i]);
  }

  return 0;
}

// актуальный размер массива

int check_actual_size(Person *target, int size) {
  int actual_size = 0;
  if (target == NULL) {
    printf("Ошибка: указатель ведет на NULL\n");
  } else {
    for (int i = 0; i < size; i++) {
      if (target[i].FirstName[0] != '\0') {
        actual_size++;
      }
    }
  }
  return actual_size;
}

// поиск первого свободной ячейки в массиве

int find_first_free(Person *target, int size) {
  for (int i = 0; i < size; i++) {
    if (target[i].FirstName[0] == '\0')
      return i;
  }
  return -1;
}

// Безопасаное чтение целого числа

int safe_read_int(char *prompt) {
  int value;
  while (1) {
    printf("%s", prompt);
    if (scanf("%d", &value) == 1) {
      while (getchar() != '\n') // очистка буффера
        ;
      return value;
    }
    printf("Ошибка ввода. Введите целое число.\n");
    while (getchar() != '\n') // очистка буффера
      ;
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

// чтение файла

int read_file(Person *target, int max_size) {
    // открываем только для чтения
    int fd = open("persons.dat", O_RDONLY);
    if (fd == -1) {
        return 0;  // файла нет — список пуст
    }

    int count = 0;
    ssize_t bytes;
    // проверяем соотвествие считанного размеру структуры и максимальный размер
    while (count < max_size && 
           (bytes = read(fd, &target[count], sizeof(Person))) == sizeof(Person)) {
        count++;
    }

    if (bytes == -1) {
        // ошибка чтения
        close(fd);
        return -1;
    }

    close(fd);
    return count;  // сколько контактов прочитано
}

// запись в файл

int write_file(Person *target, int size) {
    // открываем файл для записи + создаем если нет + обнуляем его, последнее - новые права на файл
    int fd = open("persons.dat", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        return -1;  // не удалось открыть
    }

    for (int i = 0; i < size; i++) {
        // запись в файл
        ssize_t written = write(fd, &target[i], sizeof(Person));
        // если не соответсвует размеру - закрываем файл
        if (written != sizeof(Person)) {
            close(fd);
            return -1;  // ошибка записи
        }
    }

    close(fd);
    return size;  // успешно записано size записей
}
