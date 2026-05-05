#include "header.h"

void cycle_server(){
    key_t key = ftok("/tmp/chat_queue", 'A');
    // создание очереди
    int msqid = msgget(key, IPC_CREAT | 0666);
    msgbuf message;
    clients list;
    list.count = 0;
    
    while(1){

        // принятие сообщения со сторон сервера
        msgrcv(msqid, &message, sizeof(message.mtext), 10, 0);
        int sender;
        char msg_text[100];
        if (sscanf(message.mtext, "%d: %[^\n]", &sender, msg_text) != 2)
            continue;

        int flag = 0;
        for(int i = 0; i < list.count; i++){
            if (sender == list.numbers_clients[i]){
                flag = 1;

            }
        }

        if (flag == 0 && list.count < 100){
            list.numbers_clients[list.count] = sender;
            list.count++;
            printf("Новый клиент %d подключен\n", sender);
        }



        // принятие сигнала shutdown
        if (strcmp(msg_text, "shutdown") == 0){
            for (int i = 0; i < list.count; i++){
                if (sender == list.numbers_clients[i]){
                    for (int j = i; j < list.count - 1; j++) {
                        list.numbers_clients[j] = list.numbers_clients[j+1];
                    }
                    list.count--;
                    printf("Клиент %d отключен\n", sender);
                    break;
                }

            }
        }

        // отправка сообщения всем клиентам
        msgbuf new_message;
        strcpy(new_message.mtext, message.mtext);
        for (int i = 0; i < list.count;i++){
            new_message.mtype = list.numbers_clients[i];
            if (new_message.mtype != sender && new_message.mtype != 0){
                msgsnd(msqid, &new_message, sizeof(message.mtext), 0);
            }

        }
        

    }
}

void cycle_client(int mtype){
    key_t key = ftok("/tmp/chat_queue", 'A');

    //
    int msqid = msgget(key, 0);

    pid_t pid = fork();
    if (pid == 0){
        msgbuf message;
        while(1){
            msgrcv(msqid, &message, sizeof(message.mtext), mtype, 0);
            printf("Сообщение от сервера: %s\n", message.mtext);
        }

    } else if (pid == -1){
        perror("fork");
        return;
    }
    while(1){
        msgbuf own_message;
        own_message.mtype = 10;

        char input[128];
        safe_read_string(input, sizeof(input), "Ваше сообщение: ");
        char full_message[sizeof(own_message.mtext)];
        snprintf(full_message, sizeof(full_message), "%d: %s", mtype, input);
        strcpy(own_message.mtext, full_message);

        msgsnd(msqid, &own_message, sizeof(own_message.mtext), 0);
        if (strcmp(input, "shutdown") == 0) {
            // Даём время дочернему процессу получить последние сообщения
            usleep(100000);
            kill(pid, SIGTERM);
            break;
        }
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