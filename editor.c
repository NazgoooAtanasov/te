#include "editor.h"

void editor_init(Editor* e, SDL_Window* window) {
    e->buffer_size = 0;
    e->sdlr = scp(SDL_CreateRenderer(window, -1, 0));
    e->cursor.x = 0;
    e->cursor.y = 0;
    
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
        .x = e->cursor.x * CHAR_WIDTH * SCALE_FACTOR,
        .y = e->cursor.y * CHAR_HEIGHT * SCALE_FACTOR,
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

void editor_insert_at_cursor(const char* text, Editor* e) {
    size_t len = strlen(text);
    if (e->cursor.x == e->buffer_size) {
        memcpy(&e->text_buff[e->buffer_size], text, len);
    } else if (e->cursor.x == 0) {
        memmove(&e->text_buff[0 + len], &e->text_buff[0], e->buffer_size);
        memcpy(&e->text_buff[0], text, len);
    } else {
        memmove(&e->text_buff[e->cursor.x + len], &e->text_buff[e->cursor.x], e->buffer_size - e->cursor.x);
        memcpy(&e->text_buff[e->cursor.x], text, len);
    }
    e->buffer_size += len;
    e->cursor.x += len;
}

void editor_delete_at_cursor(Editor* e) {
    if (e->cursor.x < 0 || e->buffer_size <= 0) return;

    if (e->cursor.x < e->buffer_size && e->cursor.x > 0) {
        memmove(&e->text_buff[e->cursor.x - 1],
                &e->text_buff[e->cursor.x],
                e->buffer_size - e->cursor.x);
    }
    e->buffer_size--;
    e->cursor.x--;
}
