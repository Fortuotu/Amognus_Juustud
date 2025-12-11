#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/mman.h>

#define OVERRIDE_SIZE 6

typedef struct override_s {
    void *target;

    uint8_t original[OVERRIDE_SIZE];
    uint8_t patched[OVERRIDE_SIZE];
} override_t;

static override_t installed[8] = { 0 };
static int next_idx = 0;

int override_install(void *target, void *hook, size_t tramp_size) {
    const size_t PAGE_SIZE = 4096;

    static uint8_t shellcode[OVERRIDE_SIZE] = {
        0x68, 0x00, 0x00, 0x00, 0x00,
        0xC3
    };

    *(void **)(&shellcode[1]) = hook;

    mprotect(
        (void *)((uint32_t)target - ((uint32_t)target % PAGE_SIZE)),
        PAGE_SIZE * 2,
        PROT_EXEC | PROT_READ | PROT_WRITE
    );

    installed[next_idx].target = target;
    memcpy(installed[next_idx].original, target, OVERRIDE_SIZE);
    memcpy(installed[next_idx].patched, shellcode, OVERRIDE_SIZE);

    return next_idx++;
}

void override_enable(int idx) {
    memcpy(installed[idx].target, installed[idx].patched, OVERRIDE_SIZE);
}

void override_disable(int idx) {
    memcpy(installed[idx].target, installed[idx].original, OVERRIDE_SIZE);
}
