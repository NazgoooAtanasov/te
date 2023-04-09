#include "editor.h"

void editor_init(Editor* e, SDL_Window* window, const char* file_path) {
    e->buffer_size = 0;
    e->buffer_cursor = 0;
    e->sdlr = scp(SDL_CreateRenderer(window, -1, 0));

    e->cursor.x = 0;
    e->cursor.y = 0;

    e->file_path = NULL;
    
    { // font initialization
        Font f;
        font_load_font(e->sdlr, &f);
        e->font = f;
    }

    { // loading of proviced file by path
        if (file_path != NULL) {
            FILE* fd = fopen(file_path, "r");
            fseek(fd, 0, SEEK_END);
            int len = ftell(fd);
            fseek(fd, 0, SEEK_SET);
            char* text = malloc(sizeof(char) * len);
            fread(text, len, sizeof(char), fd);
            memcpy(e->text_buff, text, len);
            e->buffer_size = len;
            e->buffer_cursor = len;
            e->file_path = file_path;

            vec2 cur_pos = cursor_calc_end_position(e->text_buff, e->buffer_size);
            e->cursor.x = cur_pos.x;
            e->cursor.y = cur_pos.y;
        }
    }
}

void editor_render_text(Editor* e) {
    int rendering_x = 0, rendering_y = 0;
    for (size_t i = 0; i < e->buffer_size; ++i) {
        int symbol = e->text_buff[i];

        if (symbol == '\n') {
            rendering_y++;
            rendering_x = 0;
        } else {
            SDL_Rect pos = (SDL_Rect) {
                .x = rendering_x * CHAR_WIDTH * SCALE_FACTOR,
                .y = rendering_y * CHAR_HEIGHT * SCALE_FACTOR,
                .h = CHAR_HEIGHT * SCALE_FACTOR,
                .w = CHAR_WIDTH * SCALE_FACTOR
            };
            SDL_RenderCopy(e->sdlr, e->font.texture, &e->font.chars[symbol - 32], &pos);
            rendering_x++;
        }
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
    if (e->buffer_cursor == e->buffer_size) {
        memcpy(&e->text_buff[e->buffer_size], text, len);
    } else if (e->buffer_cursor == 0) {
        memmove(&e->text_buff[0 + len], &e->text_buff[0], e->buffer_size);
        memcpy(&e->text_buff[0], text, len);
    } else {
        memmove(&e->text_buff[e->buffer_cursor + len], &e->text_buff[e->buffer_cursor], e->buffer_size - e->buffer_cursor);
        memcpy(&e->text_buff[e->buffer_cursor], text, len);
    }

    e->buffer_size += len;
    e->buffer_cursor += len;

    if (*text == '\n') {
        cursor_reset_x(&e->cursor);
        cursor_move_y(&e->cursor, 1);
    } else {
        cursor_move_x(&e->cursor, len);
    }
}

void editor_delete_at_cursor(Editor* e) {
    if (e->buffer_cursor <= 0 || e->buffer_size <= 0) return;

    if (e->buffer_cursor < e->buffer_size && e->buffer_cursor > 0) {
        memmove(&e->text_buff[e->buffer_cursor - 1],
                &e->text_buff[e->buffer_cursor],
                e->buffer_size - e->buffer_cursor);
    }

    e->buffer_size--;
    e->buffer_cursor--;

    if (e->text_buff[e->buffer_cursor] == '\n') {
        cursor_reset_x(&e->cursor);
        cursor_move_y(&e->cursor, -1);

        int last_line_len = last_line_length(e->text_buff, e->buffer_size);
        cursor_move_x(&e->cursor, last_line_len);
    } else {
        cursor_move_x(&e->cursor, -1);
    }
}

void cursor_move_x(struct _cursor* c, int val) {
    c->x += val;
    if (c->x < 0) cursor_reset_x(c);
}

void cursor_move_y(struct _cursor* c, int val) {
    c->y += val;
    if (c->y < 0) cursor_reset_y(c);
}
 
void cursor_reset_x(struct _cursor* c) {
    c->x = 0;
}

void cursor_reset_y(struct _cursor* c) {
    c->y = 0;
}

vec2 cursor_calc_end_position(const char* str, size_t str_len) {
    int x = last_line_length(str, str_len);
    size_t y = 0;
    for (size_t i = 0; i < str_len; ++i) {
        if (str[i] == '\n') y++;
    }

    vec2 v2 = vec2_create(x, y);
    return v2;
}
