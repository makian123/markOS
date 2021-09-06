#include "alloc.hpp"
#include <mm/pmm.hpp>
#include <mm/vmm.hpp>
#include "mem.hpp"
#include <math.hpp>
#include <proc/mutex.hpp>

struct alloc_metadata {
	size_t pages;
	size_t size;
};

DECLARE_LOCK(lib_lock);

void *alloc(size_t size) {
	size_t page_count = DIV_ROUNDUP(size, PAGE_SIZE);

	void *ptr = (char *)pmm_allocz(page_count + 1);

	if (!ptr)
		return NULL;

	ptr += MEM_PHYS_OFFSET;

	struct alloc_metadata *metadata = (alloc_metadata*)ptr;
	ptr += PAGE_SIZE;

	metadata->pages = page_count;
	metadata->size = size;

	return ptr;
}

void free(void *ptr) {
	struct alloc_metadata *metadata = (alloc_metadata*)ptr - PAGE_SIZE;

	pmm_free((void *)metadata - MEM_PHYS_OFFSET, metadata->pages + 1);
}

void *realloc(void *ptr, size_t new_size) {
	if (!ptr)
		return alloc(new_size);

	// Reference metadata page
	struct alloc_metadata *metadata = (alloc_metadata*)ptr - PAGE_SIZE;

	if (DIV_ROUNDUP(metadata->size, PAGE_SIZE) ==
		DIV_ROUNDUP(new_size, PAGE_SIZE)) {
		metadata->size = new_size;
		return ptr;
	}

	void *new_ptr = alloc(new_size);
	if (new_ptr == NULL)
		return NULL;

	if (metadata->size > new_size) {
		// Copy all the data from the old pointer to the new pointer, within the
		// range specified by 'size'.
		memcpy(new_ptr, ptr, new_size);
	} else {
		memcpy(new_ptr, ptr, metadata->size);
	}

	free(ptr);

	return new_ptr;
}

void *liballoc_alloc(size_t size) {
	return (void*)pmm_allocz(size) + MEM_PHYS_OFFSET;
}

int liballoc_free(void *ptr, size_t size) {
	pmm_free(ptr - MEM_PHYS_OFFSET, size);
	return 0;
}

int liballoc_lock() {
	LOCK(lib_lock);
	asm volatile("cli");
	return 0;
}

int liballoc_unlock() {
	UNLOCK(lib_lock);
	asm volatile("sti");
	return 0;
}