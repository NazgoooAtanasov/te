#ifndef EDITOR_H_
#define EDITOR_H_

#include <SDL2/SDL.h>
#include <stdlib.h>
#include "font.h"

struct _cursor {
    size_t x;
    size_t y;
};

#define TEXT_BUFF_CAP 1024
typedef struct _editor {
    char text_buff[TEXT_BUFF_CAP];
    size_t buffer_size;

    Font font;
    SDL_Renderer* sdlr;

    struct _cursor cursor;
} Editor;

void editor_init(Editor* e, SDL_Window* window);
void editor_render_text(Editor* e);
void editor_render_cursor(Editor* e);
void editor_free(Editor* e);
void editor_insert_at_cursor(const char* text, Editor* e);
void editor_delete_at_cursor(Editor* e);

#endif // EDITOR_H_
