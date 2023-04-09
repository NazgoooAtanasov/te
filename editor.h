#ifndef EDITOR_H_
#define EDITOR_H_

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include "font.h"
#include "stringutil.h"

struct _cursor {
    int x;
    int y;
};

void cursor_reset_x(struct _cursor* c);
void cursor_reset_y(struct _cursor* c);
void cursor_move_x(struct _cursor* c, int val);
void cursor_move_y(struct _cursor* c, int val);

#define TEXT_BUFF_CAP 1024
typedef struct _editor {
    char text_buff[TEXT_BUFF_CAP];
    size_t buffer_size;
    size_t buffer_cursor;

    Font font;
    SDL_Renderer* sdlr;

    struct _cursor cursor;

    const char* file_path;
} Editor;

void editor_init(Editor* e, SDL_Window* window, const char* file_path);
void editor_render_text(Editor* e);
void editor_render_cursor(Editor* e);
void editor_free(Editor* e);
void editor_insert_at_cursor(const char* text, Editor* e);
void editor_delete_at_cursor(Editor* e);

#endif // EDITOR_H_
