#include <include/bitmap.hpp>

bool isBitmapSet(Bitmap *bmp, size_t len){
    return (bmp->buffer[len / 8] & 1 << (len % 8)) != 0;
}

void setBitmap(Bitmap *bmp, size_t len){
    bmp->buffer[len / 8] |= 1 << (len % 8);
}

void unsetBitmap(Bitmap *bmp, size_t len){
    bmp->buffer[len / 8] &= ~(1 << (len % 8));
}

void flipBitmap(Bitmap *bmp, size_t len){
    bmp->buffer[len / 8] ^= 1 << (len % 8);
} 