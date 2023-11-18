#ifndef S21_CAT_UTILITIES_H
#define S21_CAT_UTILITIES_H

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  int number_non_empty;  // b
  int show_end_str;      // e
  int number_all;        // n
  int compress;          // s
  int show_tabs;         // t
  int disp_non_print;    // v
} Flags;

int CatParser(int argc, char *argv[], Flags *flags);
int CatWithArgs(char *argv[], Flags *flags);
void CatNoArgs(int file_desc);

#endif