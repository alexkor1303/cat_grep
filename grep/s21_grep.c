#include "s21_grep.h"
int main(int argc, char *argv[]) {
  regex_t regex;
  Flags flags = {false, false, false, false, false,
                 false, false, false, false, false};
  if (argc >= 3) {
    parseFlags(argc, argv, &flags, &regex);
    readAndPrintFile(argc, argv, &flags, &regex);
  } else {
    fprintf(stderr,
            "usage: grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
            "[-C[num]]\n");
    fprintf(stderr,
            "\t[-e pattern] [-f file] [--binary-files=value] [--color=when]\n");
    fprintf(stderr,
            "\t[--context[=num]] [--directories=action] [--label] "
            "[--line-buffered]\n");
    fprintf(stderr, "\t[--null] [pattern] [file ...]\n");
    return 1;
  }
  regfree(&regex);
  return 0;
}

void parseFlags(int argc, char *argv[], Flags *flags, regex_t *regex) {
  int currentFlag = 0;
  int regFlag = 0;
  char pattern[1024] = {0};

  while ((currentFlag = getopt_long(argc, argv, "e:ivclnhsof:", NULL, NULL)) !=
         (-1)) {
    switch (currentFlag) {
      case 'e':
        if (regFlag == 1) {
          strcat(pattern, "|");
        }
        flags->e = true;
        strcat(pattern, optarg);
        if (regFlag == 0) {
          regFlag = 1;
        }
        break;
      case 'i':
        flags->i = true;
        regFlag = REG_ICASE;
        break;
      case 'v':
        flags->v = true;
        break;
      case 'c':
        flags->c = true;
        break;
      case 'l':
        flags->l = true;
        break;
      case 'n':
        flags->n = true;
        break;
      case 'h':
        flags->h = true;
        break;
      case 's':
        flags->s = true;
        break;
      case 'f':
        flags->f = true;
        break;
      case 'o':
        flags->o = true;
        break;
    }
  }
  if (flags->e) {
    regcomp(regex, pattern, regFlag);
  } else {
    regcomp(regex, argv[optind], regFlag);
    optind++;
  }
}

void readAndPrintFile(int argc, char *argv[], Flags *flags, regex_t *regex) {
  size_t len = 0;
  regmatch_t pmatch[1];
  FILE *file;
  char *line = NULL;
  int read = 0;
  int status = 0;

  int strMatchCntFlagC = 0;
  int filesCount = 0;

  filesCount = argc - optind;

  while (optind < argc) {
    file = fopen(argv[optind], "r");
    if (file) {
      int flagFilePrinted = 0;
      int strMatchCnt = 0;
      while ((read = getline(&line, &len, file)) != -1) {
        status = regexec(regex, line, 0, NULL, 0);
        strMatchCnt++;
        status = regexec(regex, line, 1, pmatch, 0);
        if ((flags->v && status == REG_NOMATCH) ||
            (status == 0 && (flags->v == false || flags->e))) {
          if (filesCount > 1 && flags->h == false && flags->c == false &&
              flags->l == false) {
            printf("%s:", argv[optind]);
          }
          if (flags->n && flags->c == false && flags->l == false) {
            printf("%d:%s", strMatchCnt, line);
            if (line[strlen(line) - 1] != '\n') {
              printf("\n");
            }
          } else if (flags->c) {
            strMatchCntFlagC++;
          } else if (flags->l && flagFilePrinted == 0) {
            printf("%s\n", argv[optind]);
            flagFilePrinted++;
          } else if (flags->l == false) {
            printf("%s", line);
            if (line[strlen(line) - 1] != '\n') {
              printf("\n");
            }
          }
        }
      }
      if (flags->o && status == 0 && optind < argc - 1) {
        processFlagO(line, pmatch, 1);
      }
      if (flags->c) {
        if (filesCount > 1 && flags->h == false && flags->l == false) {
          printf("%s:%d\n", argv[optind], strMatchCntFlagC);
        } else if (flags->l == false) {
          printf("%d\n", strMatchCntFlagC);
        }
        strMatchCntFlagC = 0;
        if (filesCount >= 1 && flags->l == true) {
          printf("%s:%d\n", argv[optind], strMatchCntFlagC);
        }
      }
    } else {
      if (flags->s == 0) {
        fprintf(stderr, "s21_grep: %s: No such file or directory\n",
                argv[optind]);
      }
    }
    optind++;
    if (file) {
      fclose(file);
      file = NULL;
    }
  }
  if (line) free(line);
}

void processFlagO(const char *line, regmatch_t *matches, int count) {
  for (int i = 0; i < count; ++i) {
    if (matches[i].rm_so != -1) {
      printf("%.*s\n", (int)(matches[i].rm_eo - matches[i].rm_so),
             line + matches[i].rm_so);
    }
  }
}
