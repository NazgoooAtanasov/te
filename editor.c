#include "editor.h"

void editor_init(Editor* e, SDL_Window* window) {
    e->buffer_size = 0;
    e->sdlr = scp(SDL_CreateRenderer(window, -1, 0));
    
    { // font initialization
        Font f;
        font_load_font(e->sdlr, &f);
        e->font = f;
    }
}

void editor_render_text(Editor* e) {
    for (size_t i = 0; i < e->buffer_size; ++i) {
        int symbol = e->text_buff[i];
        SDL_Rect pos = (SDL_Rect) {
            .x = i * CHAR_WIDTH * SCALE_FACTOR,
            .y = 0 * CHAR_HEIGHT * SCALE_FACTOR,
            .h = CHAR_HEIGHT * SCALE_FACTOR,
            .w = CHAR_WIDTH * SCALE_FACTOR
        };
        SDL_RenderCopy(e->sdlr, e->font.texture, &e->font.chars[symbol - 32], &pos);
    }
}

void editor_render_cursor(Editor* e) {
    SDL_Rect dest = {
        .x = 0 * CHAR_WIDTH * SCALE_FACTOR,
        .y = 0 * CHAR_HEIGHT * SCALE_FACTOR,
        .h = CHAR_HEIGHT * SCALE_FACTOR,
        .w = CHAR_WIDTH * SCALE_FACTOR
    };

    scc(SDL_SetRenderDrawBlendMode(e->sdlr, SDL_BLENDMODE_BLEND));
    scc(SDL_SetRenderDrawColor(e->sdlr, 0xd3, 0xd3, 0xd3, 0xcc));
    scc(SDL_RenderDrawRect(e->sdlr, &dest));
    scc(SDL_RenderFillRect(e->sdlr, &dest));
    scc(SDL_SetRenderDrawColor(e->sdlr, 0x00, 0x00, 0x00, 0x00));
    scc(SDL_SetRenderDrawBlendMode(e->sdlr, SDL_BLENDMODE_NONE));
}

void editor_free(Editor* e) {
    font_free(&e->font);
    SDL_DestroyRenderer(e->sdlr);
}


