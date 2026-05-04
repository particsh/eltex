#include "header.h"

void command_cycle(){

    // объявляепм буффер
    char line[1024];
    Pipeline pl = { .commands = NULL, .ncommands = 0, .max_ncommands = 0 };

    while(1){
        // привественое сообщение
        printf(">) ");

        // очистка буффера после printf
        fflush(stdout);
        // считывание ввода
        if (fgets(line, sizeof(line), stdin)) {
            // добавление в конец нуль символа
            line[strcspn(line, "\n")] = '\0';
        }
        // объявляем пайплайн
        // режем строку + выполняем пайплайн
        if (separate_pipeline(line, &pl)){
            do_pipeline(pl); 
        }
        // очищаем память
        free_pipeline(&pl);
    } 
}


// разбираем команду
int separate_command(char* input, Command *cmd) {
    cmd->append = 0;
    cmd->input_file[0] = '\0';
    cmd->output_file[0] = '\0';
    int i = 0;

    char *saveptr;
    // используем эту версию функции из GNU дабы не захломлять глобальную переменную
    char *token = strtok_r(input, " \t", &saveptr);

    while (token && i < MAX_ARGS - 1) {
        if (strcmp(token, ">") == 0) {
            token = strtok_r(NULL, " \t", &saveptr);
            if (token) {
                strncpy(cmd->output_file, token, PATH_MAX - 1);
                cmd->output_file[PATH_MAX - 1] = '\0'; // нуль симвло для безопасности
            }
        } else if (strcmp(token, ">>") == 0) {
            token = strtok_r(NULL, " \t", &saveptr);
            if (token) {
                strncpy(cmd->output_file, token, PATH_MAX - 1);
                cmd->output_file[PATH_MAX - 1] = '\0'; // нуль симвло для безопасности
                cmd->append = 1;
            }
        } else if (strcmp(token, "<") == 0) {
            token = strtok_r(NULL, " \t", &saveptr);
            if (token) {
                strncpy(cmd->input_file, token, PATH_MAX - 1);
                cmd->input_file[PATH_MAX - 1] = '\0'; // нуль симвло для безопасности
            }
        } else {
            cmd->argv[i++] = strdup(token);   // вместо присваивания для безопасности
        }
        token = strtok_r(NULL, " \t", &saveptr);
    }
    cmd->argv[i] = NULL;
    return 0;
}



// разбираем пайплайн
int separate_pipeline(char* input, Pipeline *pl){
    char *token = strtok(input, "|");
    while (token != NULL) {
        // Если массив заполнен, расширяем
        if (pl->ncommands == pl->max_ncommands) {
            int new_cap = (pl->max_ncommands == 0) ? 1 : pl->max_ncommands * 2;
            Command *tmp = realloc(pl->commands, new_cap * sizeof(Command));
            if (!tmp) return -1; // не удалось выделить память
            pl->commands = tmp;
            pl->max_ncommands = new_cap;
        }
        char cmdbuf[1024];
        strncpy(cmdbuf, token, sizeof(cmdbuf) - 1);
        cmdbuf[sizeof(cmdbuf)-1] = '\0';

        if (separate_command(cmdbuf, &pl->commands[pl->ncommands]) != 0) {
            return -1;
        }
        pl->ncommands++;
        token = strtok(NULL, "|");
    }
    return pl->ncommands;

}

int do_pipeline(Pipeline pl) {
    int n = pl.ncommands;
    if (n == 0) return 0;


    // открываем пайпы
    int pipes[n-1][2];
    for (int i = 0; i < n-1; i++) {
        // в случае ошибки выходим
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            return -1;
        }
    }

    pid_t pids[n];
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            return -1;
        }
        if (pid == 0) {
            // Дочерний процесс
            // Перенаправление из/в трубы
            if (i > 0) {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }
            if (i < n-1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            // Закрываем все дескрипторы труб в ребёнке
            for (int j = 0; j < n-1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // Перенаправление из файла (<)
            if (pl.commands[i].input_file[0] != '\0') {
                int fd_in = open(pl.commands[i].input_file, O_RDONLY);
                if (fd_in == -1) {
                    perror(pl.commands[i].input_file);
                    _exit(EXIT_FAILURE); // моментальный выход
                }
                if (dup2(fd_in, STDIN_FILENO) == -1) {
                    perror("dup2 stdin");
                    _exit(EXIT_FAILURE); // моментальный выход
                }
                close(fd_in);
            }

            // Перенаправление в файл (>, >>)
            if (pl.commands[i].output_file[0] != '\0') {
                int flags = O_WRONLY | O_CREAT;
                flags |= pl.commands[i].append ? O_APPEND : O_TRUNC; // добавление флагов для >>
                int fd_out = open(pl.commands[i].output_file, flags, 0644);
                if (fd_out == -1) {
                    perror(pl.commands[i].output_file);
                    _exit(EXIT_FAILURE); // моментальный выход
                }
                if (dup2(fd_out, STDOUT_FILENO) == -1) {
                    perror("dup2 stdout");
                    _exit(EXIT_FAILURE); // моментальный выход
                }
                close(fd_out);
            }

            // непосредственный запуск команды
            execvp(pl.commands[i].argv[0], pl.commands[i].argv);
            perror("execvp");
            _exit(EXIT_FAILURE); // моментальный выход
        } else {
            pids[i] = pid;
        }
    }

    // Родитель закрывает все трубы
    for (int i = 0; i < n-1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Ожидание завершения всех детей
    for (int i = 0; i < n; i++) {
        waitpid(pids[i], NULL, 0);
    }

    return 0;
}

// очистка пайплайна
void free_pipeline(Pipeline *pl) {
    for (int i = 0; i < pl->ncommands; i++) {
        for (int j = 0; pl->commands[i].argv[j] != NULL; j++) {
            free(pl->commands[i].argv[j]);
        }
    }
    free(pl->commands);
    pl->commands = NULL;
    pl->ncommands = pl->max_ncommands = 0;
}