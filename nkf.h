#ifndef __NKF_H_LOADED__
#define __NKF_H_LOADED__

#include <stdio.h>

#undef getc
#define getc(f) \
    gonkf_getc(f)
#undef ungetc
#define ungetc(c,f) \
    gonkf_ungetc(c,f)
#undef putchar
#define putchar(c) \
    gonkf_putchar(c)

#undef TRUE
#undef FALSE

static int
gonkf_getc(FILE *f);

static int
gonkf_ungetc(int c, FILE *f);

static void
gonkf_putchar(int c);

static void
gonkf_no_memory_error();

unsigned char *
gonkf_convert(unsigned char *str, int str_size, char *opts, int opts_size);

const char *
gonkf_convert_guess(unsigned char *str, int str_size);

#endif // __NKF_H_LOADED__
