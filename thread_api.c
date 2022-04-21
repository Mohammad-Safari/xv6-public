#include "stddef.h"
#include "types.h"
#include "stat.h"
#include "user.h"

void *allocate_aligned(size_t size, size_t align);
int thread_creator(void (*fn)(void *), void *arg);

int thread_creator(void (*fn)(void *), void *arg)
{
    void *stack = (void *)allocate_aligned(4096, 4096);
    int tid = thread_create(stack);
    if (tid < 0)
    {
        return -1;
    }
    else if (tid == 0)
    {
        (fn)(arg);
        free(stack);
        exit();
    }

    return tid;
}

void *allocate_aligned(size_t size, size_t align)
{
    if (!size || !align || (align & (align - 1)))
        return NULL;
    // e.g. 100 alignment will mask 011 in addresses
    const size_t mask = align - 1;
    // allocate memory for the size + align
    const uint memptr = (uint)(malloc(size + align));
    // allocation failure
    if (!memptr)
        return NULL;
    // calculate the aligned memory address
    return (void *)((memptr + mask) & ~mask);
    // e.g. allocated address:0x09 => (0x09 + 0b011) & 0b11100 =
    // 0b10010 & 0b11100 = 0b1000 => aligned address:0x10
}