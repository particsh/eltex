#include "header.h"

void cycle(char* name_sender, char* name_receive, int flag){
    struct mq_attr attr;
    attr.mq_maxmsg  = 10;
    attr.mq_msgsize = 256;
    mqd_t mq_output = mq_open(name_sender, O_RDWR | O_CREAT, 0600, &attr);
    mqd_t mq_input = mq_open(name_receive, O_RDONLY | O_CREAT, 0600, &attr);
    char message_output[128];
    char message_input[128];
    int mq_prio = 0;
    if (flag == 0){
        while(1){
            safe_read_string(message_output, 128, "Введите сообщение: ");
            mq_send(mq_output, message_output, strlen(message_output) + 1, 0);
            mq_receive(mq_input, message_input, strlen(message_input) + 1, &mq_prio);
            printf("Сообщение: %s\n", message_input);
            if (mq_prio == 255){
                break;
            }
        }
    } else{
        while(1){
            mq_receive(mq_input, message_input, strlen(message_input) + 1, &mq_prio);
            printf("Сообщение: %s\n", message_input);
            if (mq_prio == 255){
                break;
            }
            safe_read_string(message_output, 128, "Введите сообщение: ");
            mq_send(mq_output, message_output, strlen(message_output) + 1, 0);
        }       
    }
    mq_close(mq_output);
    mq_close(mq_input);
    mq_unlink(name_sender);
}
void cycle_pong(){
   

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