#include <stdio.h>
#include <stdbool.h>
#include "watcher.h"
#include "brightness.h"
#include "countof.h"
#include "bluetooth.h"
static bool dimmed = false;
static bool disabled = false;
void ignition_on_callback(void) {
    disabled = false;
    set_brightness(dimmed? 0x01:0xff);
}
void ignition_off_callback(void) {
    disabled = true;
    set_brightness(0x00);
}
void headlights_on_callback(void) {
    dimmed = true;
    set_brightness(disabled? 0x00:0x01);
}
void headlights_off_callback(void) {
    dimmed = false;
    set_brightness(disabled? 0x00:0xff);
}
int main(void) {
    struct watcher watchers[] = {
        { 5,    ignition_off_callback,      ignition_on_callback    },
        { 25,   headlights_off_callback,    headlights_on_callback  },
    };
    return watch_lines(watchers, countof(watchers));
}
