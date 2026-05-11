#include "header.h"


// основной цикл программы: flag == 0 — отправитель, flag != 0 — принимающая сторона
void cycle(char* name_sender, char* name_receive, int flag){
    if (name_sender == NULL || name_sender[0] != '/' ||
        name_receive == NULL || name_receive[0] != '/') {
        fprintf(stderr, "Имена очередей должны начинаться с '/'\n");
        exit(EXIT_FAILURE);
    }
    struct mq_attr attr = {0}; // структкура сообщения
    attr.mq_maxmsg  = MAX_MSG ; // максимальное количество сообщений
    attr.mq_msgsize = MAX_MSG_SIZE; // максимальная длина сообщения
    mqd_t mq_output = (mqd_t)-1; // переменная для проверки на ошибку
    mqd_t mq_input  = (mqd_t)-1; // переменная для проверки на ошибку
    // открываем очередь на исходящих сообщений с флагами на создание и чтение/запись
    mq_output = mq_open(name_sender, O_RDWR | O_CREAT, 0600, &attr);
    if (mq_output == (mqd_t)-1) { // защита от ошибки
        perror("mq_open output");
        exit(EXIT_FAILURE);
    }
    // открываем вторую очередь для входящих сообщений с флагами на создание и чтение/запись
    mq_input = mq_open(name_receive, O_RDWR | O_CREAT, 0600, &attr);
    if (mq_input == (mqd_t)-1) { // защита от ошибки
        perror("mq_open input");
        mq_close(mq_output);
        exit(EXIT_FAILURE);
    }
    char message[MAX_MSG_SIZE]; // само сообщение
    unsigned int mq_prio = 0; // Приоритет 255 используется как служебный сигнал завершения диалога
    if (flag == 0){ // цикл если мы отправляющая сторона
        while(1){
            safe_read_string(message, MAX_MSG_SIZE, "Введите сообщение: "); // читаем ввод от пользователя
            if (strcmp(message, "quit") == 0){ // если выход - отправляем код EXIT_PRIORITY для выхода из цикла
                if (mq_send(mq_output, message, strlen("quit") + 1, EXIT_PRIORITY) == -1){ // защита от ошибки
                    perror("mq_send");
                    break;
                }
                break;
            } else{ // если нет просто отправляем сообщение 
                if (mq_send(mq_output, message, strlen(message) + 1, 0) == -1){ // защита от ошибки
                    perror("mq_send"); 
                    break;
                }
            }
            // принимаем сообщение
            if (mq_receive(mq_input, message, MAX_MSG_SIZE, &mq_prio) == -1){ // защита от ошибки
                perror("mq_receive");
                break;
            }
            // проверяем приорите принятого сообщения, если EXIT_PRIORITY - выходим
            if (mq_prio == EXIT_PRIORITY){
                break;
            }
            printf("Сообщение: %s\n", message);
        }

    } else{ // цикл если мы принимающая сторона
        while(1){
            // принимаем сообщение
            if (mq_receive(mq_input, message, MAX_MSG_SIZE, &mq_prio) == -1){ // защита от ошибки
                perror("mq_receive");
                break;
            }
            if (mq_prio == EXIT_PRIORITY){ // если приоритет EXIT_PRIORITY - отправляем такую же ответку, дабы не залезть в вечное ожидание
                if (mq_send(mq_output, "quit", strlen("quit") + 1, EXIT_PRIORITY) == -1){ // защита от ошибки
                    perror("mq_send");
                    break;
                }
                break;
            }
            printf("Сообщение: %s\n", message); 
            safe_read_string(message, MAX_MSG_SIZE, "Введите сообщение: "); // читаем ввод от пользователя
            if (strcmp(message, "quit") == 0){ // если выход - отправляем код EXIT_PRIORITY для выхода из цикла
                if (mq_send(mq_output, message, strlen(message) + 1, EXIT_PRIORITY) == -1){ // защита от ошибки
                    perror("mq_send");
                    break;
                }
                break;
            } else{ // просто отпрвляем сообщение
                if (mq_send(mq_output, message, strlen(message) + 1, 0) == -1){ // защита от ошибки
                    perror("mq_send");
                    break;
                }
            }
        }       
    }
    // закрываем дескрипторы очередей
    mq_close(mq_output); 
    mq_close(mq_input); 
    // удаляем имена очередей сообщений из системы
    mq_unlink(name_sender); 
    mq_unlink(name_receive); 
}


// безопасное чтение строки + сообщение
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