#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "brightness.h"
int set_brightness(unsigned val){
    char brightness_path[] = "/sys/class/backlight/10-0045/brightness";
    int fd = open(brightness_path, O_WRONLY);
    if(fd < 0) return perror("open"), -1; 
    char buf[4];
    unsigned len = snprintf(buf, sizeof(buf), "%d", val);
    int rc = write(fd, buf, len);
    close(rc);
    return (rc < 0)? -1:0;
}
int dimmer_control(enum dimmer_state state){
    switch(state){
        case dark:      return  set_brightness(0x00);
        case dim:       return  set_brightness(0x01);
        case bright:    return  set_brightness(0xff);
        default:        return  -1;
    }
}
