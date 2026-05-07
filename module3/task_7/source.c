#include "header.h"

void cycle(char* name_sender, char* name_receive, int flag){
    struct mq_attr attr;
    attr.mq_maxmsg  = 100;
    attr.mq_msgsize = 256;
    mqd_t mq_output = (mqd_t)-1;
    mqd_t mq_input  = (mqd_t)-1;
    mq_output = mq_open(name_sender, O_RDWR | O_CREAT, 0600, &attr);
    if (mq_output == (mqd_t)-1) {
        perror("mq_open output");
        exit(EXIT_FAILURE);
    }
    mq_input = mq_open(name_receive, O_RDONLY | O_CREAT, 0600, &attr);
    if (mq_input == (mqd_t)-1) {
        perror("mq_open input");
        mq_close(mq_output);
        exit(EXIT_FAILURE);
    }
    char message[256];
    unsigned int mq_prio = 0;
    if (flag == 0){
        while(1){
            safe_read_string(message, 128, "Введите сообщение: ");
            if (strcmp(message, "quit") == 0){
                if (mq_send(mq_output, message, sizeof(message), 255) == -1){
                    perror("mq_send");
                    break;
                }
            } else{
                if (mq_send(mq_output, message, sizeof(message), 0) == -1){
                    perror("mq_send");
                    break;
                }
            }
            if (mq_receive(mq_input, message, sizeof(message), &mq_prio) == -1){
                perror("mq_receive");
                break;
            }
            if (mq_prio == 255){
                break;
            }
            printf("Сообщение: %s\n", message);
        }

    } else{
        while(1){
            if (mq_receive(mq_input, message, sizeof(message), &mq_prio) == -1){
                perror("mq_receive");
                break;
            }
            if (mq_prio == 255){
                if (mq_send(mq_output, "quit", sizeof(message), 255) == -1){
                    perror("mq_send");
                    break;
                }
                break;
            }
            printf("Сообщение: %s\n", message);
            safe_read_string(message, 128, "Введите сообщение: ");
            if (strcmp(message, "quit") == 0){
                if (mq_send(mq_output, message, sizeof(message), 255) == -1){
                    perror("mq_send");
                    break;
                }
                break;
            } else{
                if (mq_send(mq_output, message, sizeof(message), 0) == -1){
                    perror("mq_send");
                    break;
                }
            }
        }       
    }
    mq_close(mq_output);
    mq_close(mq_input);
    mq_unlink(name_sender);
    mq_unlink(name_receive);
}

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