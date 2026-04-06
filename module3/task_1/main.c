#include "header.h"

int main(int argc, char *argv[]){
    if (argc < 2) {
        printf("Передайте хотя бы один аргумент\n");
        return 1;
    }
    separtions_tasks(argc, argv);
    return 0;
}