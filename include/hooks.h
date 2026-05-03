#pragma once

#include <stdint.h>

void hooks_init(void (*render_hook)(void), void (*player_update_hook)(void *));
uint8_t hooks_toggle();
