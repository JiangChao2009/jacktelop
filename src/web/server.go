package main

/*
#cgo LDFLAGS: -llcm
#include <stdio.h>
#include <stdlib.h>
#include <lcm/lcm.h>
#include "test_message_t.h"

int send_message(char *txt) {
	lcm_t *lcm = lcm_create(NULL);
	if(!lcm) {
		return 1;
	}
	test_message_t msg = {.text = txt};
	test_message_t_publish(lcm, "test", &msg);
	lcm_destroy(lcm);
	return 0;
}
*/
import "C"
import "fmt"
import "unsafe"

//	"html/template"
//	"io/ioutil"
//	"net/http"

func main() {
	fmt.Printf("Hello World\n")
	cstr := C.CString("Hello World from C!")
	C.send_message(cstr)
	C.free(unsafe.Pointer(cstr))
}
