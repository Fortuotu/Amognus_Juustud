#pragma once

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

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

void vp_transform_inv(
    const float x,
    const float y,
    const float width,
    const float height,
    const float x_w,
    const float y_w,
    float *x_nd,
    float *y_nd);

void sprintf_mat4x4(char *buf, const mat4x4f_t *mat);

vec4f_t vec4_mul_mat4x4(const vec4f_t *vec, const mat4x4f_t *mat);
