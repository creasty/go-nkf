#include <setjmp.h>

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

// Load library
#define PERL_XS 1
#include "nkf/utf8tbl.c"
#include "nkf/nkf.c"

#include "nkf.h"


/*=== Utils
==============================================================================================*/
static int gonkf_isize;
static int gonkf_osize;
static unsigned char *gonkf_ibuf;
static unsigned char *gonkf_obuf;
static int gonkf_icount;
static int gonkf_ocount;
static unsigned char *gonkf_iptr;
static unsigned char *gonkf_optr;

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
    size_t size;
    unsigned char *p;

    if (gonkf_guess_flag) {
        return;
    }

    if (gonkf_ocount--) {
        *gonkf_optr++ = c;
    } else {
        size = gonkf_osize + gonkf_osize;
        p = (unsigned char *)realloc(gonkf_obuf, size + 1);

        if (gonkf_obuf == NULL){
            gonkf_no_memory_error();
            longjmp(panic, 1);
        }

        gonkf_obuf = p;
        gonkf_optr = gonkf_obuf + gonkf_osize;
        gonkf_ocount = gonkf_osize;
        gonkf_osize = size;
        *gonkf_optr++ = c;
        gonkf_ocount--;
    }
}

static void
gonkf_no_memory_error() {
    // failed to allocate memory
}


/*=== Bindings
==============================================================================================*/
unsigned char *
gonkf_convert(unsigned char *str, int str_size, char *opts, int opts_size)
{
    gonkf_isize = str_size + 1;
    gonkf_osize = gonkf_isize * 1.5 + 256;
    gonkf_obuf = (unsigned char *)malloc(gonkf_osize);

    if (gonkf_obuf == NULL) {
        gonkf_no_memory_error();
        return NULL;
    }

    gonkf_obuf[0] = '\0';
    gonkf_ocount = gonkf_osize;
    gonkf_optr = gonkf_obuf;
    gonkf_icount = 0;
    gonkf_ibuf  = str;
    gonkf_iptr = gonkf_ibuf;
    gonkf_guess_flag = 0;

    if (setjmp(panic) == 0) {
        reinit();
        options(opts);
        kanji_convert(NULL);
    } else {
        free(gonkf_obuf);
        gonkf_no_memory_error();
        return NULL;
    }

    *gonkf_optr = 0;
    free(gonkf_obuf);

    return gonkf_obuf;
}

const char *
gonkf_convert_guess(unsigned char *str, int str_size)
{
    gonkf_isize = str_size + 1;
    gonkf_icount = 0;
    gonkf_ibuf  = str;
    gonkf_iptr = gonkf_ibuf;

    gonkf_guess_flag = 1;
    reinit();
    guess_f = 1;

    kanji_convert(NULL);

    return get_guessed_code();
}