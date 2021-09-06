#pragma once

#include <include/types.hpp>

#define FLAT_PTR(PTR) (*((int(*)[])(PTR)))

struct Bitmap{
    uint8_t *buffer;
    size_t len;
};

bool isBitmapSet(Bitmap *bmp, size_t len);
void setBitmap(Bitmap *bmp, size_t len);
void unsetBitmap(Bitmap *bmp, size_t len);
void flipBitmap(Bitmap *bmp, size_t len);

static inline bool bitmap_test(void *bitmap, size_t bit) {
	bool ret;
	asm volatile("bt %1, %2"
				 : "=@ccc"(ret)
				 : "m"(FLAT_PTR(bitmap)), "r"(bit)
				 : "memory");
	return ret;
}

static inline bool bitmap_set(void *bitmap, size_t bit) {
	bool ret;
	asm volatile("bts %1, %2"
				 : "=@ccc"(ret), "+m"(FLAT_PTR(bitmap))
				 : "r"(bit)
				 : "memory");
	return ret;
}

static inline bool bitmap_unset(void *bitmap, size_t bit) {
	bool ret;
	asm volatile("btr %1, %2"
				 : "=@ccc"(ret), "+m"(FLAT_PTR(bitmap))
				 : "r"(bit)
				 : "memory");
	return ret;
}