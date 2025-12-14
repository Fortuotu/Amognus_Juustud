#include "utils.h"

void *find_gameassembly() {
    static void *cached_ptr = NULL;

    if (cached_ptr != NULL) {
        return cached_ptr;
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

    cached_ptr = smallest;

    return smallest;
}

void vp_transform(
    const float x,
    const float y,
    const float width,
    const float height,
    const float x_nd,
    const float y_nd,
    float *x_w,
    float *y_w)
{
    const float nx = x_nd * 0.5f + 0.5f;
    const float ny = y_nd * 0.5f + 0.5f;

    *x_w = x + nx * width;
    *y_w = y + ny * height;
}

void vp_transform_inv(
    const float x,
    const float y,
    const float width,
    const float height,
    const float x_w,
    const float y_w,
    float *x_nd,
    float *y_nd)
{
    const float nx = (x_w - x) / width;
    const float ny = (y_w - y) / height;

    *x_nd = nx * 2.0f - 1.0f;
    *y_nd = ny * 2.0f - 1.0f;
}
