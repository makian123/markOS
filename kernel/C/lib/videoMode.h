#ifndef BOOT_VIDEO_H
#define BOOT_VIDEO_H

#include "types.h"
#include "system.h"

#define VIDEO_FIRST_MENU 0x0000

/* Standard BIOS video modes (BIOS number + 0x0100) */
#define VIDEO_FIRST_BIOS 0x0100

/* VESA BIOS video modes (VESA number + 0x0200) */
#define VIDEO_FIRST_VESA 0x0200

/* Video7 special modes (BIOS number + 0x0900) */
#define VIDEO_FIRST_V7 0x0900

/* Special video modes */
#define VIDEO_FIRST_SPECIAL 0x0f00
#define VIDEO_80x25 0x0f00
#define VIDEO_8POINT 0x0f01
#define VIDEO_80x43 0x0f02
#define VIDEO_80x28 0x0f03
#define VIDEO_CURRENT_MODE 0x0f04
#define VIDEO_80x30 0x0f05
#define VIDEO_80x34 0x0f06
#define VIDEO_80x60 0x0f07
#define VIDEO_GFX_HACK 0x0f08
#define VIDEO_LAST_SPECIAL 0x0f09

/* Video modes given by resolution */
#define VIDEO_FIRST_RESOLUTION 0x1000

/* The "recalculate timings" flag */
#define VIDEO_RECALC 0x8000

void store_screen(void);
#define DO_STORE() store_screen()

struct modeInfo{
    uint16_t mode;
    uint16_t x, y;
    uint16_t depth;
} modeInfo;

struct cardInfo{
    const uint8_t *cardName;
    sint32_t (*set_mode)(struct modeInfo *mode);
    sint32_t (*probe)(void);
    struct mode_info *modes;
    sint32_t nmodes;
    sint32_t unsafe;
    uint16_t xmode_first;
    uint16_t xmode_n;
} cardInfo;

sint32_t mode_defined(uint16_t mode);

#define ADAPTER_CGA	0
#define ADAPTER_EGA	1
#define ADAPTER_VGA	2

extern int adapter;
extern int force_x, force_y;
extern int do_restore;
extern int graphic_mode;

static inline uint8_t in_idx(uint16_t port, uint8_t index){
    outb(port, index);
    return inb(port+1);
}



#endif