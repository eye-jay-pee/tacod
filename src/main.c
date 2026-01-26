#define countof(x) (sizeof(x) / sizeof(x[0]))

#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <linux/gpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "watcher.h"

void ignition_on(void) {
    printf("ignition on\n");
}
void ignition_off(void) {
    printf("ignition off\n");
}
void headlights_on(void) {
    printf("headlights on\n");
}
void headlights_off(void) {
    printf("headlights off\n");
}

int main(void) {
    struct watcher watchers[] = {
        {5, ignition_off, ignition_on},
        {25, headlights_off, headlights_on},
    };

    watch_lines(watchers, countof(watchers));
}
