#include "header.h"

int num_of_tens(int num){
    int count = 0;

    if (num == 0) {
        return 1;
    }

    while (num > 0) {
        num /= 10;
        count++;
    }

    return count;
}

void processing_line(const char* line, int *max, int *min){

    if (line == NULL || max == NULL || min == NULL) {
        return;
    }

    char *copy = malloc(strlen(line) + 1);
    if (copy == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    strcpy(copy, line);

    char *saveptr = NULL;
    char* token = strtok_r(copy, " ", &saveptr);
    if (token == NULL) {
        free(copy);
        return;
    }

    int num = atoi(token);
    *max = num;
    *min = num;

    token = strtok_r(NULL, " ", &saveptr);

    while(token != NULL){
        num = atoi(token);
        if (num > *max){
            *max = num;
        } 
        if (num < *min){
            *min = num;
        } 
        token = strtok_r(NULL, " ", &saveptr);
    }
    
    free(copy);
    printf("Max: %d\nMin: %d\n", *max, *min);

}

int generate_random_line(char **line){
    size_t count_of_nums = 1 + rand() % MAX_LEN_OF_NUMS;
    size_t one_num_len = num_of_tens(MAX_NUM) + 1;
    size_t len_line = count_of_nums * one_num_len;
    *line = malloc((len_line + 1)* sizeof(char));
    if (*line == NULL){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    int num = 0;
    int used = 0;
    for (size_t i = 0; i < count_of_nums; i++){
        num = rand() % MAX_NUM;
        int written = snprintf(*line + used, len_line + 1 - used, "%d ", num);

        if (written < 0) {
            perror("snprintf");
            free(*line);
            exit(EXIT_FAILURE);
        }

        used += written;
    }

    return used;
}

void producer_cycle(char* filename){
    srand(time(NULL));
    key_t key = ftok(filename, 'S');
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }
    int semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
    if (semid >= 0){
        if (semctl(semid, 0, SETVAL, 1) == -1) {
            perror("semctl");
            exit(EXIT_FAILURE);
        }
    } else{
        semid = semget(key, 1, 0666);
        if (semid == -1) {
            perror("semget");
            exit(EXIT_FAILURE);
        }
    }
    struct sembuf lock = {0, -1, SEM_UNDO};
    struct sembuf unlock = {0, 1, SEM_UNDO};
    while(1){
        sleep(1);
        if (semop(semid, &lock, 1) == -1) {
            perror("semop lock");
            exit(EXIT_FAILURE);
        }

        FILE *file = fopen(filename, "a");
        if (file == NULL){
            perror("fopen");
            semop(semid, &unlock, 1);
            exit(EXIT_FAILURE);
        }

        char* line = NULL;

        generate_random_line(&line);

        fprintf(file, "%s\n", line);

        free(line);

        fclose(file);

        if (semop(semid, &unlock, 1) == -1) {
            perror("semop unlock");
            exit(EXIT_FAILURE);
        }
    }

}
void consumer_cycle(char* filename){
    key_t key = ftok(filename, 'S');
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }
    int semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
    if (semid >= 0){
        semctl(semid, 0, SETVAL, 1);
    } else{
        semid = semget(key, 1, 0666);
    }
    struct sembuf lock = {0, -1, SEM_UNDO};
    struct sembuf unlock = {0, 1, SEM_UNDO};
    while(1){
        sleep(1);
        if (semop(semid, &lock, 1) == -1) {
            perror("semop lock");
            exit(EXIT_FAILURE);
        }

        FILE *file = fopen(filename, "r");
        if (file == NULL) {
            perror("fopen file");
            semop(semid, &unlock, 1);
            exit(EXIT_FAILURE);
        }

        char tmpname[MAX_LINE_SIZE];
        snprintf(tmpname, sizeof(tmpname), "%s.tmp", filename);

        FILE *file_tmp = fopen(tmpname, "w");
        if (file_tmp == NULL) {
            perror("fopen tmp");
            fclose(file);
            semop(semid, &unlock, 1);
            exit(EXIT_FAILURE);
        }

        char line[MAX_LINE_SIZE];
        char line_for_processing[MAX_LINE_SIZE];
        int found = 0;
        while (fgets(line, sizeof(line), file) != NULL) {
            if (!found && strchr(line, 'a') == NULL) {
                if (line[0] == '\n' || line[0] == '\0') {
                    fputs(line, file_tmp);
                    continue;
                }
                found = 1;

                line[strcspn(line, "\n")] = '\0';

                strncpy(line_for_processing, line, sizeof(line_for_processing) - 1);
                line_for_processing[sizeof(line_for_processing) - 1] = '\0';

                fprintf(file_tmp, "%s a\n", line);
            } else {
                fputs(line, file_tmp);
            }
        }   

        fclose(file);
        fclose(file_tmp);

        if (rename(tmpname, filename) == -1) {
            perror("rename");
            remove(tmpname);
            if (semop(semid, &unlock, 1) == -1) {
                perror("semop unlock");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_FAILURE);
        }

        if (semop(semid, &unlock, 1) == -1) {
            perror("semop unlock");
            exit(EXIT_FAILURE);
        }

        if (found) {
            int max;
            int min;

            processing_line(line_for_processing, &max, &min);
        }
    }

}