#include "header.h"

void menu(Person* target, int size){
    int flag = 1;
    int choice = 0;
    while (flag){
        printf("Выберите опцию\n1. Добавить контакт\n2. Редактировать контакт\n3. Удалить контакт\n4. Посмотреть контакт\n5. Выход из меню\n");
        scanf("%d", &choice);
        switch(choice){
            case 1:
                add_person(target, size);
                break;
            case 2:
                edit_person(target, size);
                break;
            case 3:
                del_person(target, size);
                break;
            case 4:
                check_person(target, size);
                break;
            case 5:
                flag = 0;
                break;
            default:
                printf("Неверный выбор\n");

        }
    }

}

int del_person(Person* target, int size){
    int flag = 0;
    if (target == NULL){
        flag = 1;
        printf("Ошибка: указатель ведет на NULL\n");
    }
    return flag;
}
int add_person(Person* target, int size) {
    if (target == NULL) {
        printf("Ошибка: указатель ведет на NULL\n");
        return 1;
    }
    int actual = check_actual_size(target, size);
    if (actual >= size) {
        printf("Ошибка: список контактов полностью заполнен\n");
        return 1;
    }
    int position = actual;

    printf("Введите имя контакта\n");
    scanf("%19s", target[position].FirstName);
    // очистка буфера после scanf
    while (getchar() != '\n');

    printf("Введите фамилию контакта\n");
    scanf("%19s", target[position].LastName);
    while (getchar() != '\n');

    printf("Вы хотите дальше заполнять поля контакта? Д/н\n");
    char choice;
    scanf(" %c", &choice);
    while (getchar() != '\n');

    if (choice == 'Д' || choice == 'д') {
        printf("Введите отчество контакта\n");
        scanf("%19s", target[position].Patronymic);
        while (getchar() != '\n');

        printf("Введите место работы контакта\n");
        scanf("%29s", target[position].PlaceOfWork);
        while (getchar() != '\n');

        printf("Введите почты контакта, пустая строка - закончить\n");
        char tmp[60];
        int i = 0;
        while (i < 10) {
            fgets(tmp, sizeof(tmp), stdin);
            tmp[strcspn(tmp, "\n")] = '\0';  // убираем \n
            if (tmp[0] == '\0') break;
            strcpy(target[position].Emails[i], tmp);
            i++;
        }

        printf("Введите номера телефона контакта, пустая строка - закончить\n");
        i = 0;
        while (i < 10) {
            fgets(tmp, sizeof(tmp), stdin);
            tmp[strcspn(tmp, "\n")] = '\0';
            if (tmp[0] == '\0') break;
            strcpy(target[position].NumbersOfPhone[i], tmp);
            i++;
        }

        printf("Введите ссылки на соцсети контакта, пустая строка - закончить\n");
        i = 0;
        while (i < 10) {
            fgets(tmp, sizeof(tmp), stdin);
            tmp[strcspn(tmp, "\n")] = '\0';
            if (tmp[0] == '\0') break;
            strcpy(target[position].LinksSocialNetwork[i], tmp);
            i++;
        }
    }
    return 0;
}
int edit_person(Person* target, int size){
    int flag = 0;
    if (target == NULL){
        flag = 1;
        printf("Ошибка: указатель ведет на NULL\n");
    }
    return flag;
}

int check_person(Person* target, int size){
    int flag = 0;
    if (target == NULL){
        flag = 1;
        printf("Ошибка: указатель ведет на NULL\n");
    }
    return flag;
}

int check_actual_size(Person* target, int size){
    int actual_size = 0;
    if (target == NULL){
        printf("Ошибка: указатель ведет на NULL\n");
    } else{
        for (int i = 0; i < size; i++){
            if (target[i].FirstName[0] != '\0'){
                actual_size++;
            }
        }
    }
    
    return actual_size;
}