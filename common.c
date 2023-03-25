#include "common.h"

void scc(int code) {
    if (code < 0) {
        fprintf(stderr, "SDL_ERROR: %s\n", SDL_GetError());
        exit(1);
    }
}

void* scp(void* ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "SDL_ERROR: %s\n", SDL_GetError());
        exit(1);
    }
    return ptr;
}