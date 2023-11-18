CC = gcc
FLAGS = -Wall -Werror -Wextra

all: s21_grep

s21_grep: s21_grep.o
	$(CC) $(FLAGS) s21_grep.c  -o s21_grep

check:
	clang-format -style=Google -i *.c *.h

test:
	sh test_grep.sh

clean:
	rm -rf *.o s21_grep

rebuild: clean s21_grep