#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTRING 4096

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
} grepFlags;

void print_content(char *, char **, grepFlags, int, int);
int s21_readstr(char *, FILE *);
