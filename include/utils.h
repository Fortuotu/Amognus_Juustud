#pragma once

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *find_gameassembly();

void vp_transform(
    const float x,
    const float y,
    const float width,
    const float height,
    const float x_nd,
    const float y_nd,
    float *x_w,
    float *y_w);

static void vp_transform_inv(
    const float x,
    const float y,
    const float width,
    const float height,
    const float x_w,
    const float y_w,
    float *x_nd,
    float *y_nd);
