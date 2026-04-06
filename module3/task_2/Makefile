# компилятор и флаги
CC = gcc -std=c11
CFLAGS = -Wall -Wextra -Werror

# каталоги
SRC_DIR = .
TEST_DIR = tests
UNITY_DIR = unity


# файлы
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
TEST_FILES = $(wildcard $(TEST_DIR)/*.c) $(UNITY_DIR)/unity.c

# цель и тесты
TARGET = bin/app
TEST_TARGET = bin/test_runner

all: build test

build: $(SRC_FILES)
	mkdir -p bin
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC_FILES)

clean:
	rm -rf bin/ report/ *.dSYM *.out *.o
	rm -rf report/ *.dSYM

test: build
	mkdir -p bin
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(SRC_FILES) $(TEST_FILES)
	./$(TEST_TARGET)

style_check:
	clang-format -n  *.c *.h 

leaks_check:
	valgrind --tool=memcheck --leak-check=yes ./$(TEST_TARGET)
	rm *.info

cpp_check:
	cppcheck --enable=all --suppress=missingIncludeSystem ./

rebuild: clean all