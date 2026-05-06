#include "game_internal.h"

static void *call_0param(void *callee) {
    void *out = NULL;

    asm volatile (
        ".intel_syntax noprefix\n"
        "call %1\n"
        ".att_syntax prefix\n"
        : "=a" (out)
        : "r" (callee)
        : "memory"
    );

    return out;
}

static void *call_2param(void *callee, void *param1, void *param2) {
    void *out = NULL;

    asm volatile (
        ".intel_syntax noprefix\n"
        "push %1\n"
        "push %2\n"
        "call %3\n"
        "add esp, 8\n"
        ".att_syntax prefix\n"
        : "=a" (out)
        : "r" (param2), "r" (param1), "r" (callee)
        : "memory"
    );

    return out;
}

static void *call_3param(void *callee, void *param1, void *param2, void *param3) {
    void *out = NULL;

    asm volatile (
        ".intel_syntax noprefix\n"
        "push %1\n"
        "push %2\n"
        "push %3\n"
        "call %4\n"
        "add esp, 12\n"
        ".att_syntax prefix\n"
        : "=a" (out)
        : "r" (param3), "r" (param2), "r" (param1), "r" (callee)
        : "memory"
    );

    return out;
}

void internal_extract_control(void *control, player_control_t *out) {
    if (control == NULL) { return; }

    out->info = *(void **)(control + PLAYER_CONTROL_NETWORKED_PLAYER_INFO);
    out->rb = *(void **)(control + PLAYER_CONTROL_RIGIDBODY_2D);

    out->id = *(uint8_t *)(control + PLAYER_CONTROL_PLAYER_ID);
}

void internal_extract_info(void *info, player_info_t *out) {
    if (info == NULL) { return; }

    out->role = *(uint16_t *)(info + NETWORKED_PLAYER_INFO_ROLE);
}

void internal_call_info_get_player_name(void *info, void **out_sys_str) {
    if (info == NULL) { return; }

    void *sys_str = call_2param(
        find_gameassembly() + NETWORKED_PLAYER_INFO_GET_PLAYER_NAME,
        info,
        NULL
    );

    *out_sys_str = sys_str;
}

void internal_extract_rb(void *rb, game_rb_t *out) {
    if (rb == NULL) { return; }

    out->dummy = 0;
}

void internal_call_rb_get_position_injected(void *rb, vec2f_t *out_pos) {
    if (rb == NULL) { return; }

    (void)call_3param(
        find_gameassembly() + RIGIDBODY_2D_GET_POSITION_INJECTED,
        rb,
        out_pos,
        NULL
    );
}

void internal_call_camera_get_projection_matrix_injected(void *camera, mat4x4f_t *out_mat) {
    if (camera == NULL) { return; }

    (void)call_3param(
        find_gameassembly() + CAMERA_GET_PROJECTION_MATRIX_INJECTED,
        camera,
        out_mat,
        NULL
    );
}

void internal_call_camera_get_view_matrix_injected(void *camera, mat4x4f_t *out_mat) {
    if (camera == NULL) { return; }

    (void)call_3param(
        find_gameassembly() + CAMERA_GET_VIEW_MATRIX_INJECTED,
        camera,
        out_mat,
        NULL
    );
}

void internal_static_call_get_main_camera(void **camera_out) {
    void *camera = call_0param(find_gameassembly() + CAMERA_STATIC_GET_MAIN);

    *camera_out = camera;
}

void internal_extract_sys_str(void *sys_str, static_string_t *out) {
    if (sys_str == NULL) { return; }

    out->len = *(int *)(sys_str + SYSTEM_STRING_LENGTH);

    uint16_t *utf16_ptr = (uint16_t *)(sys_str + SYSTEM_STRING_DATA);

    int i = 0;

    for (; i < out->len && i < STATIC_SRING_MAX_LEN - 1; i++) {
        out->data[i] = (char)*utf16_ptr++;
    }

    out->data[i] = '\0';
}

char *internal_string_for_role(player_role_t role) {
    switch (role) {
        case CREWMATE:         return "Crewmate";
        case IMPOSTOR:         return "Impostor";
        case SCIENTIST:        return "Scientist";
        case ENGINEER:         return "Engineer";
        case GUARDIAN_ANGEL:   return "Guardian Angel";
        case SHAPESHIFTER:     return "Shapeshifter";
        case CREWMATE_GHOST:   return "Crewmate (Ghost)";
        case IMPOSTOR_GHOST:   return "Impostor (Ghost)";
        case NOISEMAKER:       return "Noisemaker";
        case PHANTOM:          return "Phantom";
        case TRACKER:          return "Tracker";
        case DETECTIVE:        return "Detective";
        case VIPER:            return "Viper";
        default:               return "Unknown";
    }
}
