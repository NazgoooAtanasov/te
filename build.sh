#!/bin/sh

set -xe

CFLAGS="`pkg-config --libs sdl2`"
LIBS="`pkg-config --cflags sdl2` -lSDL2_image"

gcc ${CFLAGS} main.c font.c common.c editor.c -o te ${LIBS}
