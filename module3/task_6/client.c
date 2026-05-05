#include "header.h"

int main(int argc, char *argv[]){
    if (argc > 2){
        printf("Передайте один аргумент\n");
    } else if (argc == 1){
        printf("Передайте хотя бы один аргумент\n");
    } else{
        cycle_client(atoi(argv[1]));
    }
    return 0;
}