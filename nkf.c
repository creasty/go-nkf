#include "nkf.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <setjmp.h>

static int
gonkf_getc(FILE *f);

static int
gonkf_ungetc(int c, FILE *f);

static void
gonkf_putchar(int c);

#define BUFFER_STRETCH 2


/*  Override and load library
-----------------------------------------------*/
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

#define PERL_XS 1
#include "nkf/config.h"
#include "nkf/utf8tbl.c"
#include "nkf/nkf.c"


/*=== Utils
==============================================================================================*/
static unsigned char *gonkf_ibuf;
static unsigned char *gonkf_obuf;
static unsigned char *gonkf_iptr;
static unsigned char *gonkf_optr;
static int gonkf_isize;
static int gonkf_osize;
static int gonkf_icount;
static int gonkf_ocount;

static jmp_buf panic;
static int gonkf_guess_flag;

static int
gonkf_getc(FILE *f)
{
    if (gonkf_icount >= gonkf_isize) return EOF;

    unsigned char c = *gonkf_iptr++;
    gonkf_icount++;
    return (int)c;
}

static int
gonkf_ungetc(int c, FILE *f)
{
    if (gonkf_icount--) {
        *(--gonkf_iptr) = c;
        return c;
    } else {
        return EOF;
    }
}

static void
gonkf_putchar(int c)
{
    if (gonkf_guess_flag) {
        return;
    }

    if (gonkf_ocount--) {
        *gonkf_optr++ = c;
    } else {
        size_t size      = gonkf_osize * BUFFER_STRETCH;
        unsigned char *p = (unsigned char *)realloc(gonkf_obuf, size + 1);

        if (gonkf_obuf == NULL){
            longjmp(panic, 1);
        }

        gonkf_obuf   = p;
        gonkf_optr   = gonkf_obuf + gonkf_osize;
        gonkf_ocount = gonkf_osize;
        gonkf_osize  = size;

        *gonkf_optr++ = c;
        gonkf_ocount--;
    }
}


/*=== Bindings
==============================================================================================*/
unsigned char *
gonkf_convert(unsigned char *str, int str_size, unsigned char *opts, int opts_size)
{
    gonkf_isize = str_size + 1;
    gonkf_osize = str_size * BUFFER_STRETCH;
    gonkf_obuf  = (unsigned char *)malloc(gonkf_osize + 1);

    if (gonkf_obuf == NULL) {
        // failed to allocate memory
        return NULL;
    }

    gonkf_obuf[0]    = '\0';
    gonkf_ocount     = gonkf_osize;
    gonkf_optr       = gonkf_obuf;
    gonkf_icount     = 0;
    gonkf_ibuf       = str;
    gonkf_iptr       = gonkf_ibuf;
    gonkf_guess_flag = 0;

    if (setjmp(panic) == 0) {
        reinit();
        options(opts);
        kanji_convert(NULL);
    } else {
        // failed to allocate memory
        free(gonkf_obuf);
        return NULL;
    }

    *gonkf_optr = 0;

    return gonkf_obuf;
}

const char *
gonkf_convert_guess(unsigned char *str, int str_size)
{
    gonkf_isize      = str_size + 1;
    gonkf_icount     = 0;
    gonkf_ibuf       = str;
    gonkf_iptr       = gonkf_ibuf;
    gonkf_guess_flag = 1;

    reinit();
    guess_f = 1;

    kanji_convert(NULL);

    return get_guessed_code();
}
