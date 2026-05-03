#include "header.h"

void command_cycle(){

    // объявляепм буффер
    char line[1024];
    char *args[64];
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
        separate_pipeline(line, &pl);
        

    } 
}


// разбираем комманду
int separate_command(char* input, Command *cmd){

    // ставим параметры по умолчанию
    cmd->append = 0;
    cmd->input_file[0] = '\0';
    cmd->output_file[0] = '\0';
    int i = 0;

    // нарезаем строку
    char *token = strtok(input, " \t");
    while (token && i < MAX_ARGS - 1){
        // если > передаем имя файла для выхода
        if (strcmp(token, ">")){
            token = strtok(NULL, " \t");
            if (token) {
                strncpy(cmd->output_file, token, PATH_MAX - 1);
            }
        }
        // если > передаем имя файла для выхода + ставим флаг на перезапись
        if (strcmp(token, ">>")){
            token = strtok(NULL, " \t");
            if (token) {
                strncpy(cmd->output_file, token, PATH_MAX - 1);
                cmd->append = 1;
            }
        }
        // если > передаем имя файла для входа
        if (strcmp(token, "<")){
            token = strtok(NULL, " \t");
            if (token) {
                strncpy(cmd->input_file, token, PATH_MAX - 1);
            }
        }else{
            // значит просто аргументы для команды
            cmd->argv[i++] = token;
        }
        token = strtok(NULL, " \t"); 
    }
    cmd->argv[i] = NULL;
    return 0;
}



// разбираем пайплайн
int separate_pipeline(char* input, Pipeline *pl){
    int flag = 0;
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
        if (separate_command(token, &pl->commands[pl->ncommands]) != 0) {
            return -1; // ошибка внутри команды
        }
        pl->ncommands++;
        token = strtok(NULL, "|");
    }
    return pl->ncommands;

}

int do_pipeline(Pipeline pl){
    int pipes[pl.ncommands - 1][2];
    for(int i = 0; i < pl.ncommands - 1;i++){
        pipe(pipes[i]);
    }
    for (int i = 0; i < pl.ncommands;i++){
        pid_t pid = fork();
        if (pid == 0){

            if (i > 0){
                dup2(pipes[i-1][0], STDIN_FILENO);
            }

            if (i < pl.ncommands - 1){
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            for(int j = 0; j < pl.ncommands - 1; j++){
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

        }
    }
    return 0;
}