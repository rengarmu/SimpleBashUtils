#include "s21_cat.h"

int main(int argc, char **argv) {
  options opt = {
      .b = 0,
      .e = 0,
      .n = 0,
      .s = 0,
      .t = 0,
      .v = 0,
  };

  int index, n_arg;

  if (argc > 1) {
    n_arg = pars_opts(argc, argv, &opt, &index);
  }
  // printf(" b:%d, e:%d, n:%d, s:%d, t:%d, v:%d, index = %d n_arg = %d\n",
  // opt.b, opt.e, opt.n, opt.s, opt.t, opt.v, index, n_arg);

  if (n_arg < argc)
    open_files(argc, argv, opt, n_arg);
  else
    fprintf(stderr, "usage: s21_cat [-benstvTE] [file ...]\n");

  return 0;
}

int pars_opts(int argc, char **argv, options *opt, int *pindex) {
  int rez;
  int n_arg = 1;

  while ((rez = getopt_long(argc, argv, "+benstvTE", long_opt, pindex)) != -1) {
    n_arg++;
    switch (rez) {
      case 'b':
        opt->b = 1;
        break;
      case 'n':
        opt->n = 1;
        break;
      case 's':
        opt->s = 1;
        break;
      case 'v':
        opt->v = 1;
        break;
      case 't':
        opt->t = 1;
        opt->v = 1;
        break;
      case 'e':
        opt->e = 1;
        opt->v = 1;
        break;
      case 'E':
        opt->e = 1;
        break;
      case 'T':
        opt->t = 1;
        break;
      default:
        fprintf(stderr, "usage: s21_cat [-benstvTE] [file ...]\n");
        break;
    }
    if (opt->b) opt->n = 0;
  }
  return n_arg;
}

void open_files(int argc, char **argv, options opt, int index) {
  FILE *f;
  while (index < argc) {
    f = fopen(argv[index], "rb");
    if (f == NULL) {
      fprintf(stderr, "s21_cat: %s: No such file or directory\n", argv[index]);
      break;
    } else {
      print_content(f, opt);
      fclose(f);
    };
    index++;
  }
}

void print_content(FILE *f, options opt) {
  int line = 1;
  char prev = '\0';  // Предыдущий символ
  int n_prev = -1;
  int n_simvol = 0;
  char simvol = '\0';
  int prev_string_is_empty = 0;
  for (n_simvol = s21_fgetc(f); n_simvol != -1;
       n_prev = n_simvol, prev = simvol, n_simvol = s21_fgetc(f)) {
    //	if (n_simvol == 10 && n_prev == 13) { continue; }
    simvol = (char)n_simvol;
    if (opt.s && n_prev == -1 && simvol == '\n' && prev_string_is_empty)
      continue;
    if (opt.s && prev == '\n' && simvol == '\n' && prev_string_is_empty)
      continue;
    if (opt.b && prev == '\n' && simvol != '\n') {
      printf("%6d\t", line++);
    }
    if (opt.b && n_prev == -1 && simvol != '\n') {
      printf("%6d\t", line++);
    }
    if (opt.n && prev == '\n') {
      printf("%6d\t", line++);
    }
    if (opt.n && n_prev == -1) {
      printf("%6d\t", line++);
    }
    if ((opt.e) && simvol == '\n') printf("$");
    //	if (opt.v && simvol == '\n') { printf("****13****SYMBOL <%c> %d ",
    // simvol, simvol); }
    if ((opt.v) && (int)simvol >= 0 && (int)simvol < 32 && n_simvol != 10 &&
        simvol != '\t') {
      printf("^%c", (int)simvol + 64);
      continue;
    }
    if (opt.v && (int)simvol == 127) {
      printf("^%c", (int)simvol - 64);
      continue;
    }
    if (opt.t && simvol == '\t') {
      printf("^I");
      continue;
    }
    printf("%c", simvol);
    if ((prev == '\n' || n_prev == -1) && simvol == '\n')
      prev_string_is_empty = 1;
    else
      prev_string_is_empty = 0;
  }
}

int s21_fgetc(FILE *f) {
  int r = fgetc(f);
  //	printf("==%d==", r);
  if (r == EOF)
    return -1;
  else
    return r;
}

int s21_readstr(char *s, FILE *f) {
  int i = 0;
  int c;
  for (c = fgetc(f); c != '\n' && c != -1; c = fgetc(f), i++) {
    if (c != -1 && c != '\n') s[i] = c;
  }
  s[i] = '\0';
  return c;
}
