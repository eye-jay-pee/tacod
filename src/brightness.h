#ifndef BRIGHTNESS_HEADER
#define BRIGHTNESS_HEADER
enum dimmer_state {
    dark, 
    dim,
    bright
};
int set_brightness(unsigned val);
int dimmer_control(enum dimmer_state state);
#endif
