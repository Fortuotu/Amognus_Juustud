#include "utils.h"

void *find_gameassembly() {
    static void *cached_addr = NULL;

    if (cached_addr != NULL) {
        return cached_addr;
    }

    FILE *maps_file = fopen("/proc/self/maps", "r");
    char *line = NULL;
    size_t size = 0;

    void *start = NULL;
    void *end = NULL;
    char filename[1028] = { 0 };

    void *smallest = NULL;

    while (getline(&line, &size, maps_file) != -1) {
        sscanf(line, "%p-%p %*s %*s %*s %*s %1027[^\n]", &start, &end, filename);
        
        if (strstr(filename, "GameAssembly.dll") != NULL) {            
            if (start < smallest || smallest == NULL) {
                smallest = start;
            }
        }
    }

    free(line);
    fclose(maps_file);

    cached_addr = smallest;

    return smallest;
}
