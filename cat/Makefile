CC = gcc
FLAGS = -Wall -Werror -Wextra

all: s21_cat

s21_cat: s21_cat.o
	$(CC) $(FLAGS) s21_cat.c  -o s21_cat

check:
	clang-format -style=Google -i *.c *.h

test:
	sh test_cat.sh

clean:
	rm -rf *.o s21_cat

rebuild: clean s21_cat