#include "header.h"

volatile sig_atomic_t sigint_count = 0; // счетчик 
static int fd; // файловый дескриптор


// обработчик сигналов
void handle_signal(int sig){
    // орбабатываем SIGINT и SIGQUIT
    static const char message[] = "Получен сигнал SIGINT или SIGQUIT!\n";
    write(fd, message, sizeof(message) - 1);
    if (sig == SIGINT){
        sigint_count++;
        if (sigint_count >= 3){
            close(fd);
            _exit(0);
        }
    }

}

// основной цикл программы
void cycle(){
    struct sigaction sa;
    // передаем функцию в обработчик сигналов
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    // чтобы не прерывался sleep
    sa.sa_flags = SA_RESTART;

    // выставляем что делать при получении SIGINT и SIGQUIT
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);



    fd = open("file_task5.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644); // открываем файл + создаем если нету
    int i = 1; // счетчик
    char counter_buff[12];
    while(1){
        int lenght_buff = int_to_string(i, counter_buff);
        write(fd, counter_buff, lenght_buff);
        write(fd, "\n", 1);
        i++;
        sleep(1); // ждем секунду
    }
}


// перевод целого в строчку
int int_to_string(int increment, char* buffer){
    char temp[12];
    int i = 0, pos = 0;

    if (increment == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return 1;
    }

    while(increment > 0){
        temp[i++] = '0' + (increment % 10);
        increment /= 10;
    }
    while(i > 0){
        buffer[pos++] = temp[--i];

    }
    buffer[pos] = '\0';
    return pos;
}