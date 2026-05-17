#include <stdio.h>
#include <stdlib.h>

#include "trampoline.h"

void *(*function_tramp)(size_t) = NULL;

void detour(size_t size) {

    function_tramp(size);
}

int main(void) {
    function_tramp = trampoline_install(malloc, detour);
    
    int *x = malloc(sizeof(int));

    free(x);

    return 0;
}
