#pragma once

#include "Zydis.h"

void *trampoline_install(void *func, void *detour);
