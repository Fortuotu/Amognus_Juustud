#include "hooks.h"
#include "override.h"
#include "renderer.h"
#include "io.h"

#include <stdio.h>
#include <dlfcn.h>

static int inited = 0;

static int glXSwapBuffers_idx;
static void (*glXSwapBuffers_orig)(void *, void *);

static renderer_t renderer;

void __glXSwapBuffers_override(void *dpy, void *drawable);

void hooks_init() {
    void *glx_so = dlopen("libGLX.so.0", RTLD_LAZY);
    glXSwapBuffers_orig = dlsym(glx_so, "glXSwapBuffers");
    glXSwapBuffers_idx = override_install(glXSwapBuffers_orig, __glXSwapBuffers_override);

    render_new(&renderer);

    inited = 1;
}

void hooks_toggle() {
    static int enabled = 0;

    if (!inited) {
        return;
    }

    if (!enabled) {
        override_enable(glXSwapBuffers_idx);
    }
    else {
        override_disable(glXSwapBuffers_idx);
    }

    enabled = !enabled;
}

void __glXSwapBuffers_override(void *dpy, void *drawable) {
    static int i = 0;
    static char buf[512];

    sprintf(buf, "#%d glXSwapBuffers(%p, %p)\n", i++, dpy, drawable);
    io_sendstr(buf);

    render_scene(&renderer);

    override_disable(glXSwapBuffers_idx);
    glXSwapBuffers_orig(dpy, drawable);
    override_enable(glXSwapBuffers_idx);
}
