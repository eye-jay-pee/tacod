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

int request_lines(int chip_fd) {
    struct gpio_v2_line_request req = {
        .offsets = {5, 25},
        .num_lines = 2,
        .config = {
            .flags = GPIO_V2_LINE_FLAG_INPUT |
                     GPIO_V2_LINE_FLAG_EDGE_RISING |
                     GPIO_V2_LINE_FLAG_EDGE_FALLING,
        },
    };

    if (ioctl(chip_fd, GPIO_V2_GET_LINE_IOCTL, &req) < 0)
        return perror("GET_LINE_IOCTL"), -1;

    return req.fd;
}

int main(void) {
    int chip_fd = open("/dev/gpiochip0", O_RDWR);
    if (chip_fd < 0) return perror("open gpiochip"), -1; 

    int line_fd = request_lines(chip_fd);
    if(line_fd < 0) return perror("request lines"), -1;

    int ep = epoll_create1(0);
    if (ep < 0) { perror("epoll_create1"); return 1; }

    struct epoll_event ev = {
        .events = EPOLLIN,
        .data.fd = line_fd,
    };

    epoll_ctl(ep, EPOLL_CTL_ADD, line_fd, &ev);

    for (;;) {
        epoll_wait(ep, &ev, 1, -1);

        struct gpio_v2_line_event ge;
        read(line_fd, &ge, sizeof(ge));

        printf("line %u %s\n",
               ge.offset,
               ge.id == GPIO_V2_LINE_EVENT_RISING_EDGE ? "rising" :
               ge.id == GPIO_V2_LINE_EVENT_FALLING_EDGE ? "falling" : "unknown");
    }
}

