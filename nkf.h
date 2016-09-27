#ifndef __NKF_H_LOADED__
#define __NKF_H_LOADED__

unsigned char *
gonkf_convert(unsigned char *str, int str_size, unsigned char *opts, int opts_size);

const char *
gonkf_convert_guess(unsigned char *str, int str_size);

#endif // __NKF_H_LOADED__
