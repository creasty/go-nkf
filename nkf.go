package nkf

/*
#cgo CFLAGS: -I .

#include <stdlib.h>
#include "nkf.h"
*/
import "C"

import (
	"errors"
	"unsafe"
)

var noMemoryError = errors.New("failed to allocate memory")

type Encoding string

const (
	ENCODING_ASCII            Encoding = "US-ASCII"
	ENCODING_ISO_8859_1       Encoding = "ISO-8859-1"
	ENCODING_ISO_2022_JP      Encoding = "ISO-2022-JP"
	ENCODING_CP50220          Encoding = "CP50220"
	ENCODING_CP50221          Encoding = "CP50221"
	ENCODING_CP50222          Encoding = "CP50222"
	ENCODING_ISO_2022_JP_1    Encoding = "ISO-2022-JP-1"
	ENCODING_ISO_2022_JP_3    Encoding = "ISO-2022-JP-3"
	ENCODING_ISO_2022_JP_2004 Encoding = "ISO-2022-JP-2004"
	ENCODING_SHIFT_JIS        Encoding = "Shift_JIS"
	ENCODING_WINDOWS_31J      Encoding = "Windows-31J"
	ENCODING_CP10001          Encoding = "CP10001"
	ENCODING_EUC_JP           Encoding = "EUC-JP"
	ENCODING_EUCJP_NKF        Encoding = "eucJP-nkf"
	ENCODING_CP51932          Encoding = "CP51932"
	ENCODING_EUCJP_MS         Encoding = "eucJP-MS"
	ENCODING_EUCJP_ASCII      Encoding = "eucJP-ASCII"
	ENCODING_SHIFT_JISX0213   Encoding = "Shift_JISX0213"
	ENCODING_SHIFT_JIS_2004   Encoding = "Shift_JIS-2004"
	ENCODING_EUC_JISX0213     Encoding = "EUC-JISX0213"
	ENCODING_EUC_JIS_2004     Encoding = "EUC-JIS-2004"
	ENCODING_UTF_8            Encoding = "UTF-8"
	ENCODING_UTF_8N           Encoding = "UTF-8N"
	ENCODING_UTF_8_BOM        Encoding = "UTF-8-BOM"
	ENCODING_UTF8_MAC         Encoding = "UTF8-MAC"
	ENCODING_UTF_16           Encoding = "UTF-16"
	ENCODING_UTF_16BE         Encoding = "UTF-16BE"
	ENCODING_UTF_16BE_BOM     Encoding = "UTF-16BE-BOM"
	ENCODING_UTF_16LE         Encoding = "UTF-16LE"
	ENCODING_UTF_16LE_BOM     Encoding = "UTF-16LE-BOM"
	ENCODING_UTF_32           Encoding = "UTF-32"
	ENCODING_UTF_32BE         Encoding = "UTF-32BE"
	ENCODING_UTF_32BE_BOM     Encoding = "UTF-32BE-BOM"
	ENCODING_UTF_32LE         Encoding = "UTF-32LE"
	ENCODING_UTF_32LE_BOM     Encoding = "UTF-32LE-BOM"
	ENCODING_BINARY           Encoding = "BINARY"
	ENCODING_UNKNOWN          Encoding = "UNKNOWN"
)

func Convert(str string, options string) (string, error) {
	cstr := (*C.uchar)(unsafe.Pointer(C.CString(str)))
	defer C.free(unsafe.Pointer(cstr))

	coptions := C.CString(options)
	defer C.free(unsafe.Pointer(coptions))

	coutput := C.gonkf_convert(cstr, C.int(len(str)), coptions, C.int(len(options)))
	if coutput == nil {
		return str, noMemoryError
	}
	defer C.free(unsafe.Pointer(coutput))

	return C.GoString((*C.char)(unsafe.Pointer(coutput))), nil
}

func Guess(str string) (Encoding, error) {
	cstr := (*C.uchar)(unsafe.Pointer(C.CString(str)))
	defer C.free(unsafe.Pointer(cstr))

	coutput := C.gonkf_convert_guess(cstr, C.int(len(str)))
	if coutput == nil {
		return ENCODING_UNKNOWN, noMemoryError
	}
	// no free for const char *

	code := C.GoString((*C.char)(unsafe.Pointer(coutput)))

	return Encoding(code), nil
}
