#include "lib/panic.h"
#include "lib/framebuffer.h"

void panic(uint8_t *file, const uint8_t function[20], size_t line, uint8_t *msg){
    FBClear();
    PrintS("---------------------------------------------------------------------------");
    PrintS("\nKERNEL PANIC\n");
    glog(SILENT, "Issue encountered\n");
    glog(SILENT, msg);
    glog(SILENT, "In %s, at %s(), line %i", file, function, line);

    asm volatile("cli");
}