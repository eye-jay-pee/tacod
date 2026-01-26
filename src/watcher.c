#include <linux/gpio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include "watcher.h"
int request_lines(int chip_fd, struct watcher* watchers, unsigned watcher_n) {
    struct gpio_v2_line_request req = {
        .num_lines = watcher_n,
        .config = {
            .flags = GPIO_V2_LINE_FLAG_INPUT |
                     GPIO_V2_LINE_FLAG_EDGE_RISING |
                     GPIO_V2_LINE_FLAG_EDGE_FALLING,
        },
    };
    for(unsigned i = 0; i < watcher_n; i++) {
        req.offsets[i] = watchers[i].offset;
        if(NULL == watchers[i].on_rise || NULL == watchers[i].on_fall) 
            return perror("invalid funtion pointer"), -1;
    }

    if(ioctl(chip_fd, GPIO_V2_GET_LINE_IOCTL, &req) < 0)
        return perror("GET_LINE_IOCTL"), -1;
    else return req.fd;
}
int watch_lines(struct watcher* watchers, unsigned watcher_n) {

    int chip_fd = open("/dev/gpiochip0", O_RDWR);
    if (chip_fd < 0) return perror("open gpiochip"), -1; 

    int line_fd = request_lines(chip_fd, watchers, watcher_n);
    if(line_fd < 0) return perror("request lines"), -1;

    int ep = epoll_create1(0);
    if (ep < 0) return perror("epoll_create1"), 1; 

    struct epoll_event ev = {
        .events = EPOLLIN,
        .data.fd = line_fd,
    };

    epoll_ctl(ep, EPOLL_CTL_ADD, line_fd, &ev);

    for (;;) {
        epoll_wait(ep, &ev, 1, -1);

        struct gpio_v2_line_event ge;
        read(line_fd, &ge, sizeof(ge));

        for(unsigned i = 0; i < watcher_n; i++){
            if(ge.offset == watchers[i].offset){
                if(ge.id == GPIO_V2_LINE_EVENT_RISING_EDGE) 
                    watchers[i].on_rise(); 
                else if(ge.id == GPIO_V2_LINE_EVENT_FALLING_EDGE)
                    watchers[i].on_fall();
                break;
            }
        }
    }
}

