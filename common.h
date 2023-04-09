#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

typedef struct {
    size_t x;
    size_t y;
} vec2;

vec2 vec2_create(size_t x, size_t y);

// sdl error handling
void scc(int code);
void* scp(void* ptr);

#endif // COMMON_H_
