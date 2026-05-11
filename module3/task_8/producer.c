#include "header.h"

int main(int argc, char *argv[]){
    if (argc == 1){
        printf("Необходимо имя файла для работы приложения\n");
    } else if(argc > 2){
        printf("Принимается только один параметр\n");
    } else{
        producer_cycle(argv[1]);
    }
    return 0;
}