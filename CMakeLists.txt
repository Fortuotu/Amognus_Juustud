cmake_minimum_required(VERSION 3.20)

project(Amognus_Juustud)

set(sources 
    src/inject.c
    src/io.c
    src/utils.c
    src/override.c
    src/hooks.c
    src/game_internal.c
    src/game_helpers.c
    src/glad.c
    src/renderer.c
)

include_directories(include)

add_library(inject SHARED ${sources})
set_target_properties(inject PROPERTIES COMPILE_OPTIONS "-m32" LINK_FLAGS "-m32")
target_link_libraries(inject libdl.so libGL.so libX11.so)