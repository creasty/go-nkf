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

func Convert(str string, options string) (string, error) {
	cstr := (*C.uchar)(unsafe.Pointer(C.CString(str)))
	defer C.free(unsafe.Pointer(cstr))

	coptions := C.CString(options)
	defer C.free(unsafe.Pointer(coptions))

	coutput := C.gonkf_convert(cstr, C.int(len(str)), coptions, C.int(len(options)))
	if coutput == nil {
		return str, errors.New("failed to convert")
	}

	defer C.free(unsafe.Pointer(coutput))
	return C.GoString((*C.char)(unsafe.Pointer(coutput))), nil
}
