#include "header.h"

volatile sig_atomic_t stop = 0;

int create_semaphores(void)
{
    int semid = semget(IPC_PRIVATE, 2, IPC_CREAT | 0600);

    if (semid == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    union semun arg;
    unsigned short values[2] = {0, 0};

    arg.array = values;

    if (semctl(semid, 0, SETALL, arg) == -1) {
        perror("semctl SETALL");
        exit(EXIT_FAILURE);
    }

    return semid;
}

void sem_wait(int semid, int sem_num)
{
    struct sembuf op;

    op.sem_num = sem_num;
    op.sem_op = -1;
    op.sem_flg = 0;

    while (semop(semid, &op, 1) == -1) {
        if (errno == EINTR) {
            continue;
        }

        perror("semop wait");
        exit(EXIT_FAILURE);
    }
}

void sem_signal(int semid, int sem_num)
{
    struct sembuf op;

    op.sem_num = sem_num;
    op.sem_op = 1;
    op.sem_flg = 0;

    if (semop(semid, &op, 1) == -1) {
        perror("semop signal");
        exit(EXIT_FAILURE);
    }
}

void remove_semaphores(int semid)
{
    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl IPC_RMID");
    }
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


    int shmid = shmget(IPC_PRIVATE, sizeof(SharedData), IPC_CREAT | 0600);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    SharedData *data = shmat(shmid, NULL, 0);
    if (data == (void *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    data->stop_flag = 0;

    int semid = create_semaphores();
    if (semid < 0){
        exit(EXIT_FAILURE);
    }


    pid_t pid = fork();
    if (pid < 0){
        perror("fork");
        exit(EXIT_FAILURE);    
    } else if(pid == 0){
        while(1){
            sleep(1);
            sem_wait(semid, DATA_READY);
            if (data->stop_flag){
                break;
            }
            processing_line(data->line, &data->max, &data->min);
            sem_signal(semid, RESULT_READY);
        }
        shmdt(data);
        exit(EXIT_SUCCESS);
    } else{
        int count = 0;
        while(!stop){
            sleep(1);
            generate_random_line(data->line, MAX_LINE_SIZE);
            sem_signal(semid, DATA_READY);
            sem_wait(semid, RESULT_READY);
            printf("Max: %d\nMin: %d\n", data->max, data->min);
            count++;
        }
        data->stop_flag = 1;
        sem_signal(semid, DATA_READY);

        waitpid(pid, NULL, 0);

        printf("Пройденых итераций: %d\n", count);
        
        shmdt(data);
        shmctl(shmid, IPC_RMID, NULL);
        remove_semaphores(semid);
    }
}