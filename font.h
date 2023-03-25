#ifndef FONT_H_
#define FONT_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "common.h"

#define ASCII_CHAR_COUNT 126 - 32
#define FONT "./font.png"
#define FONT_TEXTURE_WIDTH 192
#define FONT_TEXTURE_HEIGHT 96
#define CHAR_WIDTH (FONT_TEXTURE_WIDTH / 16)
#define CHAR_HEIGHT (FONT_TEXTURE_HEIGHT / 6) 
#define SCALE_FACTOR 2

typedef struct _font {
    SDL_Rect chars[ASCII_CHAR_COUNT];
    SDL_Texture* texture;
} Font;

void font_load_font(SDL_Renderer* renderer, Font* f);
void font_free(Font* f);

#endif // FONT_H_
