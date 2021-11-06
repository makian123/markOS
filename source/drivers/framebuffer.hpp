#pragma once

#include <include/types.hpp>
#include <stivale2.h>
#include <include/graphics.hpp>

typedef struct {
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charsize;
} PSF1_HEADER;

typedef struct {
	PSF1_HEADER* psf1_Header;
	void* glyphBuffer;
} PSF1_FONT;

typedef struct{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
}Color;

#define NORMAL  1
#define SUCCESS 2
#define WARNING 3
#define ERROR   4

uint32_t GetColor(Color col);
void setRGB(Color *col, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
Color RGB(uint8_t r, uint8_t g, uint8_t b,  uint8_t a = 255);
Color HEX2RGB(uint32_t hex);

class Framebuffer{
    private:
        Color colorScheme[8];
        Color foreground;
        Color background;
        stivale2_struct_tag_framebuffer *thisFB;
        int cursorX, cursorY;
        int font;
        void DrawHorizontalLine(long startX, long startY, long dx, Color col);
        void DrawVerticalLine(long startX, long startY, long dy, Color col);
        void DrawDiagonalLine(long startX, long startY, long endX, long endY, Color col);
        
    public:
        void Initialize(Color scheme[8], stivale2_struct_tag_framebuffer *info, int fontSpecifier);
        void SetColors(Color fg, Color bg);
        void DrawPixel(int x, int y, uint32_t color);
        uint32_t GetPixel(int x, int y);
        void DrawLine(long startX, long startY, long endX, long endY, Color col);
        void DrawTriangle(Point2 first, Point2 second, Point2 third, Color col);
        void DrawTriangleFilled(Point2 first, Point2 second, Point2 third, Color col); 
        void DrawRect(int x, int y, int w, int h, Color col);
        void DrawRectFilled(int x, int y, int w, int h, Color fill);
        void DrawCircle(int cx, int cy, int rad, Color col);
        void DrawCircleFilled(int cx, int cy, int rad, Color col);
        void Clear();

        void PrintC(uint8_t input);
        void PrintC(char input);
        void PrintS(const uint8_t *input);
        void PrintS(const char *input);
        void PrintF(uint8_t *c, ...);
        void PrintF(char *c, ...);
        void Log(int type, uint8_t *c, ...);
        void Log(int type, char *c, ...);
        void Panic(uint8_t *c, ...);

        void PrintBMP(int xPos, int yPos, BITMAP bmp);
        void CursorMove(uint8_t ch);
        Color mouseBuffer[16 * 16];
        bool mouseDrawn;

        uint16_t monitorSpecs[2];
};