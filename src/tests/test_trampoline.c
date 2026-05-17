#include <stdio.h>
#include <stdlib.h>

#include "trampoline.h"

int main(void) {
    printf("%s\n", (char *)trampoline_install(NULL, NULL));
    
    return 0;
}
