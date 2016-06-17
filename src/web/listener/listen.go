package main

/*
#cgo LDFLAGS: -llcm
#include <stdio.h>
#include <stdlib.h>
#include <lcm/lcm.h>
#include "test_message_t.h"

static void handler(const lcm_recv_buf_t *rbuf, const char *chan, const test_message_t *msg, void *user) {
	printf("Received message: [%s] on channel: [%s]\n", msg->text, chan);

}

int listen() {
	lcm_t *lcm = lcm_create(NULL);
	if(!lcm) {
		return 1;
	}
	test_message_t_subscribe(lcm, "test", &handler, NULL);
	while(1) {
		lcm_handle(lcm);
	}
	lcm_destroy(lcm);
	return 0;
}
*/
import "C"
import "fmt"

func main() {
	fmt.Printf("listening...")
	C.listen()
}
