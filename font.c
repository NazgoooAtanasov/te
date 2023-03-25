#include "./font.h"

void font_load_font(Font* f) {
    size_t i = 0;
    int x = 0, y = 0;
    while (i < ASCII_CHAR_COUNT) {
        if (x >= FONT_TEXTURE_WIDTH) {
            x = 0;
            y += CHAR_HEIGHT;
        }

        f->chars[i] = (SDL_Rect) {
            .x = x,
            .y = y,
            .w = FONT_TEXTURE_WIDTH / 18,
            .h = CHAR_HEIGHT
        };

        x += CHAR_WIDTH;
        i++;
    }
}

