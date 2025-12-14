#pragma once

#include <stdint.h>

void hooks_init(void (*render_hook)(void), void (*player_update_hook)(void *));
void hooks_toggle();
