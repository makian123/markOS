#include "lib/main.h"
#include "lib/conversion.h"
#include "lib/string.h"
#include "lib/malloc.h"
#include "lib/system.h"
#include "lib/osMath.h"
#include "lib/lists.h"
#include "lib/stivale2.h"
#include "lib/panic.h"
#include <stdarg.h>

const uint8_t keycode_ascii[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0,
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0,
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '#',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    0, 0, 0
};

void main(struct stivale2_struct *info, Framebuffer *self){
    glog(SUCCESS, "Boot Successful\n");
    PrintS("Welcome to markOS\n");
    for(;;){
    }
}