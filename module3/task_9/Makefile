# компилятор и флаги
CC = gcc -std=c11
CFLAGS = -Wall -Wextra -Werror

# каталоги
SRC_DIR = .
TEST_DIR = tests


# файлы
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# цель и тесты
TARGET = bin/app
TEST_TARGET = bin/test_runner

all: build

build: $(SRC_FILES)
	mkdir -p bin
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC_FILES)

clean:
	rm -rf bin/ report/ *.dSYM *.out *.o
	rm -rf report/ *.dSYM

style_check:
	clang-format -n  *.c *.h 

leaks_check:
	valgrind --tool=memcheck --leak-check=yes ./$(TARGET)
	rm *.info

cpp_check:
	cppcheck --enable=all --suppress=missingIncludeSystem ./

rebuild: clean all