#include "s21_grep.h"

int main(int argc, char *argv[]) {
  grepFlags flagi;
  flagi.e = flagi.i = flagi.v = flagi.c = flagi.l = flagi.n = flagi.h =
      flagi.s = 0;
  int errors = 0;
  int need_mem = 0;
  char *shablons[MAXSTRING];
  char *files[MAXSTRING];
  int ks = 0;
  int kf = 0;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-') {
      need_mem = strlen(argv[i]) + 1;
      files[kf] = (char *)malloc(need_mem);
      strcpy(files[kf], argv[i]);
      // printf("FIND FILE %s\n", files[kf]);
      kf++;
      continue;
    }
    if (argv[i][0] == '-' && argv[i][2] == '\0') {
      switch (argv[i][1]) {
        case 'e':
          flagi.e = 1;
          if (i >= argc - 1) {
            fprintf(stderr, "Error: No shablon after key -e\n");
            errors++;
            i = argc + 1;
            break;
          }
          need_mem = strlen(argv[i + 1]) + 1;
          shablons[ks] = (char *)malloc(need_mem);
          strcpy(shablons[ks], argv[i + 1]);
          // printf("FIND SHABLON %s\n", shablons[ks]);
          ks++;
          i++;
          break;
        case 'i':
          flagi.i = 1;
          break;
        case 'v':
          flagi.v = 1;
          break;
        case 'c':
          flagi.c = 1;
          break;
        case 'l':
          flagi.l = 1;
          break;
        case 'n':
          flagi.n = 1;
          break;
        case 'h':
          flagi.h = 1;
          break;
        case 's':
          flagi.s = 1;
          break;
        default:
          fprintf(stderr, "error key %s\n", argv[i]);
          errors++;
      }
      if (flagi.c == 1 || flagi.l == 1) flagi.n = 0;
    }
    if (argv[i][0] == '-' && argv[i][2] != '\0') {
      fprintf(stderr, "Error key %s\n", argv[i]);
      errors++;
    }
  }
  if (errors) {
    fprintf(stderr, "usage: s21_grep [-e:ivcln] [shablon...] [file ...]\n");
    for (int k = 0; k < ks; k++) free(shablons[k]);
    for (int k = 0; k < kf; k++) free(files[k]);
    return 1;
  }
  if (kf == 0) {
    fprintf(stderr, "usage: s21_grep [-e:ivcln] [shablon...] [file ...]\n");
    for (int k = 0; k < ks; k++) free(shablons[k]);
    return 1;
  }
  int first_file = 0;
  if (ks == 0) {
    if (kf < 2) {
      fprintf(stderr, "usage: s21_grep [-e:ivcln] [shablon...] [file ...]\n");
      for (int k = 0; k < kf; k++) free(files[k]);
      return 1;
    } else {
      need_mem = strlen(files[0]) + 1;
      shablons[ks] = (char *)malloc(need_mem);
      strcpy(shablons[ks], files[0]);
      ks++;
      first_file = 1;
    }
  }
  for (int k = first_file; k < kf; k++) {
    // printf("myFile: %s, files[k]: %s\n", myFile, files[k]);
    print_content(files[k], shablons, flagi, ks, kf - first_file);
  }

  for (int k = 0; k < ks; k++) free(shablons[k]);
  for (int k = 0; k < kf; k++) free(files[k]);
  return 0;
}

void print_content(char *myFile, char **shablons, grepFlags opt, int ks,
                   int kf) {
  if (ks == 0) {
    fprintf(stderr, "No sbalons for finds!\n");
    return;
  }

  regex_t regex;
  FILE *f;
  char str[MAXSTRING];
  int rrs;
  int l = 1;
  int rez;
  int kolvo_strok = 0;

  f = fopen(myFile, "rb");
  if (f == NULL) {
    if (!opt.s)
      fprintf(stderr, "s21_grep: %s: No such file or directory\n", myFile);
    return;
  }

  for (rrs = s21_readstr(str, f); rrs != -1; rrs = s21_readstr(str, f), l++) {
    int sovpalo = 0;
    for (int k = 0; k < ks; k++) {
      int ret;
      if (opt.i)
        ret = regcomp(&regex, shablons[k], REG_EXTENDED | REG_ICASE);
      else
        ret = regcomp(&regex, shablons[k], REG_EXTENDED);
      if (ret != 0) {
        fprintf(stderr, "Failed to compile regex %s\n", shablons[k]);
        continue;
      }
      rez = regexec(&regex, str, 0, NULL, 0);
      if (rez == 0) sovpalo++;
      regfree(&regex);
    }
    if (sovpalo && !opt.v) kolvo_strok++;
    if (!sovpalo && opt.v) kolvo_strok++;
    if (!opt.c && !opt.l) {
      if (opt.v && !sovpalo) {
        if (opt.n) {
          if (kf > 1 && !opt.h)
            printf("%s:%d:%s\n", myFile, l, str);
          else
            printf("%d:%s\n", l, str);
        } else {
          if (kf > 1 && !opt.h)
            printf("%s:%s\n", myFile, str);
          else
            printf("%s\n", str);
        }
      }
      if (!opt.v && sovpalo) {
        if (opt.n) {
          if (kf > 1 && !opt.h)
            printf("%s:%d:%s\n", myFile, l, str);
          else
            printf("%d:%s\n", l, str);
        } else {
          if (kf > 1 && !opt.h)
            printf("%s:%s\n", myFile, str);
          else
            printf("%s\n", str);
        }
      }
    }  // end of if (!opt.c)
  }
  if (opt.c) {
    if (opt.l) {
      int na_ekran = kolvo_strok > 0 ? 1 : 0;
      if (kf > 1 && !opt.h)
        printf("%s:%d\n", myFile, na_ekran);
      else
        printf("%d\n", na_ekran);
    } else {
      if (kf > 1 && !opt.h)
        printf("%s:%d\n", myFile, kolvo_strok);
      else
        printf("%d\n", kolvo_strok);
    }
  }
  if (opt.l && kolvo_strok > 0) printf("%s\n", myFile);
  fclose(f);
}

int s21_readstr(char *s, FILE *f) {
  int i = 0;
  int c;
  for (c = fgetc(f); c != '\n' && c != -1; c = fgetc(f), i++) {
    if (c != -1 && c != '\n') s[i] = c;
  }
  s[i] = '\0';
  if (c == -1 && i == 0)
    return c;
  else
    return 0;
}

/* 69
///////////////  Тестовый вывод на экран
    ouputFlags(flagi);
    printf("Our shablons: ");
    for (int k = 0; k < ks; k++)
        {
         printf("%s", shablons[k]);
                if (k < ks-1) printf(", "); else printf("\n");
        }
    printf("Our files: ");
    for (int k = 0; k < kf; k++)
        {
         printf("%s", files[k]);
                if (k < kf-1) printf(", "); else printf("\n");
        }
//////////////// Конец тестового вывода на экран
*/
