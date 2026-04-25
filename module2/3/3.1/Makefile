# компилятор и флаги
CC = gcc -std=c11
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lm

# каталоги
SRC_DIR = .


# файлы
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# цель
TARGET = bin/app

all: build 

build: $(SRC_FILES)
	mkdir -p bin
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC_FILES) $(LDFLAGS)

clean:
	rm -rf bin/ *.out *.o

style_check:
	clang-format -n  *.c *.h 

leaks_check:
	valgrind --tool=memcheck --leak-check=yes ./$(TARGET)
	rm *.info

cpp_check:
	cppcheck --enable=all --suppress=missingIncludeSystem ./

rebuild: clean all