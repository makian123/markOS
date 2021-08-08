#include "lib/main.h"
#include "lib/conversion.h"
#include "lib/string.h"
#include "lib/malloc.h"
#include "lib/system.h"
#include "lib/osMath.h"
#include "lib/lists.h"
#include "lib/stivale2.h"
#include "lib/panic.h"
#include "lib/keyboard.h"
#include <stdarg.h>

void main(struct stivale2_struct *info, Framebuffer *self){
    glog(SUCCESS, "Boot Successful\n");
    PrintS("Welcome to markOS\n");
    PrintF("\nString: %s\nChar: %c\nDecimal: %d", "string", 'c', 123);
    for(;;){
        DrawScreen();
    }
}