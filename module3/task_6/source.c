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
        int sender; // id отправившего
        char msg_text[100]; // само сообщение
        
        // парсинг сообщения
        if (sscanf(message.mtext, "%d: %[^\n]", &sender, msg_text) != 2)
            continue; // пропускаем в случае не соотвествия шаблону

        int flag = 0;
        // проверка есть ли уже клиент в списке
        for(int i = 0; i < list.count; i++){
            if (sender == list.numbers_clients[i]){
                flag = 1;

            }
        }
        // если нету - добавляем
        if (flag == 0 && list.count < 100){
            list.numbers_clients[list.count] = sender;
            list.count++;
            printf("Новый клиент %d подключен\n", sender);
        }



        // принятие сигнала shutdown
        if (strcmp(msg_text, "shutdown") == 0){
            for (int i = 0; i < list.count; i++){
                if (sender == list.numbers_clients[i]){
                    // смещаем список дабы не образововались дыры
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
            if (new_message.mtype != sender){
                // пересылаем всем кроме отправителя
                msgsnd(msqid, &new_message, sizeof(message.mtext), 0);
            }

        }
        

    }
}

void cycle_client(int mtype){
    
    key_t key = ftok("/tmp/chat_queue", 'A');

    // подключаемся к готовой очереди
    int msqid = msgget(key, 0);

    pid_t pid = fork();
    if (pid == 0){
        // принимаем сообщения в дочернем процессе
        msgbuf message;
        while(1){
            msgrcv(msqid, &message, sizeof(message.mtext), mtype, 0);
            printf("Сообщение от сервера: %s\n", message.mtext);
        }
        _exit(0);

    } else if (pid == -1){
        // выходим в случае ошибки
        perror("fork");
        return;
    }
    while(1){
        // отправляем сообщения в родительском процессе
        msgbuf own_message;
        // так как направлен на сервер
        own_message.mtype = 10;

        char input[100];
        // читаем сообщение от клиента
        safe_read_string(input, sizeof(input), "Ваше сообщение: ");
        char full_message[128];
        // добавляем подпись от клиента
        snprintf(full_message, sizeof(full_message), "%d: %s", mtype, input);
        strcpy(own_message.mtext, full_message);
        // отправляем сообщения на сервер
        msgsnd(msqid, &own_message, sizeof(own_message.mtext), 0);
        if (strcmp(input, "shutdown") == 0) {
            // Даём время дочернему процессу получить последние сообщения
            sleep(15);
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