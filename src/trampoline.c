#include "trampoline.h"

void *trampoline_install(void *func, void *detour) {
    return (void *)"trampoline_install called";
}
