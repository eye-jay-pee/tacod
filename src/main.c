#include <stdio.h>
#include "watcher.h"
#include "brightness.h"
#include "countof.h"
void ignition_on(void) {
    backlight_on();
    printf("ignition on\n");
}
void ignition_off(void) {
    backlight_off();
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
