# компилятор и флаги
CC = gcc -std=c11
CFLAGS = -Wall -Wextra -Werror

# каталоги
SRC_DIR = .
TEST_DIR = tests
UNITY_DIR = unity


# файлы
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# цель и тесты
TARGET1 = bin/ping
TARGET2 = bin/pong

all: build 

build: $(SRC_FILES)
	mkdir -p bin
	$(CC) $(CFLAGS) -o $(TARGET1) ping.c source.c
	$(CC) $(CFLAGS) -o $(TARGET2) pong.c source.c

clean:
	rm -rf bin/ report/ *.dSYM *.out *.o
	rm -rf report/ *.dSYM

style_check:
	clang-format -n  *.c *.h 

leaks_check:
	valgrind --tool=memcheck --leak-check=yes ./$(TARGET1)
	rm *.info

cpp_check:
	cppcheck --enable=all --suppress=missingIncludeSystem ./

rebuild: clean all