#include "hooks.h"
#include "override.h"
#include "renderer.h"
#include "io.h"

#include <stdio.h>
#include <dlfcn.h>

typedef struct hooks_s {
    uint8_t loaded;
    uint8_t enabled;

    void *glx_so;
    void (*glXSwapBuffers_orig)(void *, void *);
    int glXSwapBuffers_idx;

    void (*render_hook)(void);
} hooks_t;

static hooks_t hooks = { 0 };

void glXSwapBuffers_override(void *dpy, void *drawable);

void hooks_init(void (*render_hook)(void)) {
    hooks.glx_so = dlopen("libGLX.so.0", RTLD_LAZY);
    hooks.glXSwapBuffers_orig = dlsym(hooks.glx_so, "glXSwapBuffers");
    hooks.glXSwapBuffers_idx = override_install(hooks.glXSwapBuffers_orig, glXSwapBuffers_override);

    hooks.loaded = true;
}

void hooks_toggle() {
    if (!hooks.loaded) {
        return;
    }

    if (!hooks.enabled) {
        override_enable(hooks.glXSwapBuffers_idx);
    }
    else {
        override_disable(hooks.glXSwapBuffers_idx);
    }

    hooks.enabled = !hooks.enabled;
}

static void glXSwapBuffers_override(void *dpy, void *drawable) {
    hooks.render_hook();

    override_disable(hooks.glXSwapBuffers_idx);
    glXSwapBuffers_orig(dpy, drawable);
    override_enable(hooks.glXSwapBuffers_idx);
}
