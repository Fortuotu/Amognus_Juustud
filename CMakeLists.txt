cmake_minimum_required(VERSION 3.20)

project(Amognus_Juustud)

option(BUILD_TESTS "Build test executables" ON)

include_directories(include)

if(BUILD_TESTS) 
    set(SOURCES
        src/tests/test_trampoline.c
        src/trampoline.c

        src/Zydis.c
    )

    add_executable(test_trampoline ${SOURCES})
    target_compile_options(test_trampoline PRIVATE -m32 -g)
    target_link_options(test_trampoline PRIVATE -m32)
endif()

set(SOURCES 
    src/inject.c
    src/io.c
    src/utils.c
    src/override.c
    src/hooks.c
    src/game_internal.c
    src/game_helpers.c
    src/renderer.c

    src/Zydis.c
    src/glad.c
)

add_library(inject SHARED ${SOURCES})
set_target_properties(inject PROPERTIES COMPILE_OPTIONS "-m32" LINK_FLAGS "-m32")
target_link_libraries(inject libdl.so libGL.so libX11.so)
