#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"

#pragma once

#ifndef TEXT_RENDERER
#define TEXT_RENDERER

void initTextRenderer(SDL_Renderer *rend, SDL_Texture *fontTexture);
void setTextWrapping(int maxWidth);
void renderText(char *text, double fontSize, int x, int y);

#endif
