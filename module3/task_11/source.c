#include "header.h"

volatile sig_atomic_t stop = 0;

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

}

int generate_random_line(char *line, size_t size){
    size_t count_of_nums = 1 + rand() % MAX_LEN_OF_NUMS;
    int num = 0;
    int used = 0;
    for (size_t i = 0; i < count_of_nums; i++){
        num = rand() % MAX_NUM;
        int written = snprintf(line + used, size - used, "%d ", num);

        if (written < 0) {
            perror("snprintf");
            exit(EXIT_FAILURE);
        }

        if ((size_t)written >= size - used) {
            break;
        }

        used += written;
    }

    return used;
}

void handle_signal(int sig){
    if (sig == SIGINT){
        stop = 1;
    }
}
void cycle(){
    srand(time(NULL));

    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    int fd = shm_open(NAME_SHM, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, sizeof(SharedData)) == -1){
        close(fd);
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }


    SharedData *data = mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (data == MAP_FAILED) {
        close(fd);
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    close(fd);

    if (sem_init(&data->data_ready, 1, 0) == -1) {
        perror("sem_init data_ready");
        exit(EXIT_FAILURE);
    }

    if (sem_init(&data->result_ready, 1, 0) == -1) {
        perror("sem_init result_ready");
        exit(EXIT_FAILURE);
    }

    data->stop_flag = 0;

    pid_t pid = fork();
    if (pid < 0){
        perror("fork");
        exit(EXIT_FAILURE);    
    } else if(pid == 0){
        signal(SIGINT, SIG_IGN);
        while(1){
            sleep(1);
            if (sem_wait(&data->data_ready) == -1) {
                perror("sem_wait data_ready");
                exit(EXIT_FAILURE);
            }   
            if (data->stop_flag){
                break;
            }
            processing_line(data->line, &data->max, &data->min);
            if (sem_post(&data->result_ready) == -1) {
                perror("sem_post result_ready");
                exit(EXIT_FAILURE);
            }   
        }
        exit(EXIT_SUCCESS);
    } else{
        int count = 0;
        while(!stop){
            sleep(1);
            generate_random_line(data->line, MAX_LINE_SIZE);
            if (sem_post(&data->data_ready) == -1){
                perror("sem_post data_ready");
                exit(EXIT_FAILURE);
            }
            if (sem_wait(&data->result_ready) == -1){
                if (errno == EINTR && stop) {
                    break;
                }
                perror("sem_wait result_ready");
                exit(EXIT_FAILURE);
            }
            printf("Max: %d\nMin: %d\n", data->max, data->min);
            count++;
        }
        data->stop_flag = 1;
        if (sem_post(&data->data_ready) == -1){
            perror("sem_post data_ready");
            exit(EXIT_FAILURE);
        }

        waitpid(pid, NULL, 0);

        printf("Пройденных итераций: %d\n", count);
        sem_destroy(&data->data_ready);
        sem_destroy(&data->result_ready);
        if (munmap(data, sizeof(SharedData)) == -1){
            perror("munmap");
            exit(EXIT_FAILURE);
        }
        if (shm_unlink(NAME_SHM) == -1) {
            perror("shm_unlink");
        }
    }
}