#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 600

#define FONT "./font.png"
#define FONT_TEXTURE_WIDTH 192
#define FONT_TEXTURE_HEIGHT 96
#define CHAR_WIDTH (FONT_TEXTURE_WIDTH / 16)
#define CHAR_HEIGHT (FONT_TEXTURE_HEIGHT / 6) 
#define SCALE_FACTOR 2

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

#define ASCII_CHAR_COUNT 126 - 32
typedef struct _font {
    SDL_Rect chars[ASCII_CHAR_COUNT];
} Font;

char buffer[1024];
size_t buffer_count = 0;

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

void render_text(const char* text, size_t buffer_count, SDL_Renderer* renderer, SDL_Texture* texture, Font* f) {
    for (size_t i = 0; i < buffer_count; ++i) {
        int symbol = text[i];
        SDL_Rect pos = (SDL_Rect) {
            .x = i * CHAR_WIDTH * SCALE_FACTOR,
            .y = 0 * CHAR_HEIGHT * SCALE_FACTOR,
            .h = CHAR_HEIGHT * SCALE_FACTOR,
            .w = CHAR_WIDTH * SCALE_FACTOR
        };
        SDL_RenderCopy(renderer, texture, &f->chars[symbol - 32], &pos);
    }
}

void render_cursor(SDL_Renderer* renderer) {
    SDL_Rect dest = {
        .x = 0 * CHAR_WIDTH * SCALE_FACTOR,
        .y = 0 * CHAR_HEIGHT * SCALE_FACTOR,
        .h = CHAR_HEIGHT * SCALE_FACTOR,
        .w = CHAR_WIDTH * SCALE_FACTOR
    };

    scc(SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND));
    scc(SDL_SetRenderDrawColor(renderer, 0xd3, 0xd3, 0xd3, 0xcc));
    scc(SDL_RenderDrawRect(renderer, &dest));
    scc(SDL_RenderFillRect(renderer, &dest));
    scc(SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00));
    scc(SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE));
}

// @TODO: handle deleting in the middle of the line
int main(void) {
    scc(SDL_Init(SDL_INIT_VIDEO));

    SDL_Window* window = scp(SDL_CreateWindow("te", 0, 0, WINDOW_HEIGHT, WINDOW_WIDTH, SDL_WINDOW_RESIZABLE));
    SDL_Renderer* renderer = scp(SDL_CreateRenderer(window, -1, 0));
    SDL_Texture* texture = scp(IMG_LoadTexture(renderer, FONT));

    int h,w;
    scc(SDL_QueryTexture(texture, NULL, NULL, &w, &h));

    Font f;
    font_load_font(&f);

    bool should_run = true;
    while(should_run) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_KEYDOWN: {
                    switch(event.key.keysym.sym) {
                        case SDLK_ESCAPE: {
                            should_run = false;
                        } break;

                        case SDLK_RETURN: {
                        } break;

                        case SDLK_BACKSPACE: {
                            if (buffer_count > 0) {
                                buffer_count--;
                            }
                        } break;

                        /* case SDLK_LEFT: { */
                        /*     if (c.curr_x > 0) { */
                        /*         c.curr_x--; */
                        /*     } */
                        /* } break; */

                        /* case SDLK_RIGHT: { */
                        /*     if (c.curr_x < buffer_count) { */
                        /*         c.curr_x++; */
                        /*     } */
                        /* } break; */
                   }
                } break;

                case SDL_QUIT: {
                    should_run = false;
                } break;

                case SDL_TEXTINPUT: {
                    const char* text = event.text.text;
                    size_t len = strlen(text);
                    for (size_t i = 0; i < len; ++i) {
                        buffer[buffer_count] = text[i];
                        buffer_count++;
                    }
                } break;
            }
        }

        SDL_RenderClear(renderer);
        render_text(buffer, buffer_count, renderer, texture, &f);
        render_cursor(renderer);
		SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
