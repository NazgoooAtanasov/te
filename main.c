#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "editor.h"
#include "common.h"

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 600

// @TODO: handle deleting in the middle of the line
int main(void) {
    scc(SDL_Init(SDL_INIT_VIDEO));

    SDL_Window* window = scp(SDL_CreateWindow("te", 0, 0, WINDOW_HEIGHT, WINDOW_WIDTH, SDL_WINDOW_RESIZABLE));
    Editor e;
    editor_init(&e, window);

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
                            if (e.buffer_size > 0) {
                                e.buffer_size--;
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
                        e.text_buff[e.buffer_size] = text[i];
                        e.buffer_size++;
                    }
                } break;
            }
        }

        SDL_RenderClear(e.sdlr);
        editor_render_text(&e);
        editor_render_cursor(&e);
		SDL_RenderPresent(e.sdlr);
    }


    editor_free(&e);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
