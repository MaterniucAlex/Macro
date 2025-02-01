#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"

#pragma once

#ifndef TEXT_RENDERER
#define TEXT_RENDERER

void initTextRenderer(SDL_Renderer *rend, SDL_Texture *fontTexture);
void renderText(char *text, int fontSize, int x, int y);
void renderTextWrapped(char *text, int fontSize, int x, int y, int pxWrap);

#endif
