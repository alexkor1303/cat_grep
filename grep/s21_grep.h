#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  bool e;
  bool i;
  bool v;
  bool c;
  bool l;
  bool n;
  bool h;
  bool s;
  bool f;
  bool o;
} Flags;

void processFlagO(const char *line, regmatch_t *matches, int count);
void parseFlags(int argc, char *argv[], Flags *flags, regex_t *regex);
void readAndPrintFile(int argc, char *argv[], Flags *flags, regex_t *regex);

#endif
