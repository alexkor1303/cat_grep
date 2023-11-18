#include "s21_cat_utilities.h"

int main(int argc, char *argv[]) {
  if (argc == 1) {
    CatNoArgs(STDIN_FILENO);
  } else {
    int err = 1;
    Flags flags = {
        .number_non_empty = 0,
        .show_end_str = 0,
        .number_all = 0,
        .compress = 0,
        .show_tabs = 0,
        .disp_non_print = 0,
    };
    err = CatParser(argc, argv, &flags);
    if (flags.number_non_empty == 1) {
      flags.number_all = 0;
    }
    while (optind < argc) {
      err = CatWithArgs(argv, &flags);
      optind++;
    }
    return err;
  }
}

void CatNoArgs(int file_desc) {
  char buf[4096];
  int bytes_read;
  bytes_read = read(file_desc, buf, sizeof(buf));
  while (bytes_read > 0) {
    printf("%.*s", bytes_read, buf);
    bytes_read = read(file_desc, buf, sizeof(buf));
  }
}

int CatParser(int argc, char *argv[], Flags *flags) {
  int opt;
  int ret = 0;
  int index = 0;
  struct option options[] = {
      {"number-nonblank", 0, 0, 'b'},
      {"number", 0, 0, 'n'},
      {"squeeze-blank", 0, 0, 's'},
      {0, 0, 0, 0},
  };

  while ((opt = getopt_long(argc, argv, "bEnsTvet", options, &index)) != -1) {
    switch (opt) {
      case 'b':
        flags->number_non_empty = 1;
        break;

      case 'E':
        flags->show_end_str = 1;
        break;

      case 'n':
        flags->number_all = 1;
        break;

      case 's':
        flags->compress = 1;
        break;

      case 'T':
        flags->show_tabs = 1;
        break;

      case 'v':
        flags->disp_non_print = 1;
        break;

      case 'e':
        flags->show_end_str = 1;
        flags->disp_non_print = 1;
        break;

      case 't':
        flags->show_tabs = 1;
        flags->disp_non_print = 1;
        break;

      case '?':
        ret = -1;
        break;
      default:
        printf("ERROR");
        break;
    }
  }
  return ret;
}

int CatWithArgs(char *argv[], Flags *flags) {
  FILE *file;
  int ret = 0;

  file = fopen(argv[optind], "r");

  if (file != NULL) {
    int str_count = 1;
    int empty_str_count = 0;
    int last_sym = '\n';

    while (true) {
      int cur_sym = fgetc(file);
      if (cur_sym == EOF) {
        break;
      }
      if (flags->compress && cur_sym == '\n' && last_sym == '\n') {
        empty_str_count++;
        if (empty_str_count > 1) {
          continue;
        }
      } else {
        empty_str_count = 0;
      }
      if (last_sym == '\n' &&
          ((flags->number_non_empty && cur_sym != '\n') || flags->number_all)) {
        printf("%6d\t", str_count++);
      }
      if (flags->show_tabs && cur_sym == '\t') {
        printf("^");
        cur_sym = 'I';
      }
      if (flags->show_end_str && cur_sym == '\n') {
        printf("$");
      }
      if (flags->disp_non_print) {
        if ((cur_sym >= 0 && cur_sym < 9) || (cur_sym > 10 && cur_sym < 32)) {
          printf("^");
          cur_sym += 64;
        }
        if (cur_sym == 127) {
          printf("^?");
        }
        if (cur_sym > 127) {
          printf("M-^%c", cur_sym - 128);
        }
      }
      printf("%c", cur_sym);
      last_sym = cur_sym;
    }
    fclose(file);
  } else {
    ret = -1;
  }
  return ret;
}