#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTRING 1024 * 32

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} options;

static struct option long_opt[] = {{"number-nonblank", 0, NULL, 'b'},
                                   {"number", 0, NULL, 'n'},
                                   {"squeeze-blank", 0, NULL, 's'},
                                   {NULL, 0, NULL, 0}};

int pars_opts(int argc, char **argv, options *opt, int *);
void open_files(int argc, char **argv, options opt, int index);
void print_content(FILE *f, options opt);
void print_opt(options opt);
void print_args(int argc, char **argv);
int s21_fgetc(FILE *f);
int s21_readstr(char *s, FILE *f);
void trans_str(char *s_in, char *s_out, options opt);