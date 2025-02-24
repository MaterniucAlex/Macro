#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"

#pragma once

#ifndef TEXT_RENDERER
#define TEXT_RENDERER

void initTextRenderer(SDL_Renderer *rend, SDL_Texture *fontTexture);
void closeTextRenderer();
void setTextWrapping(int maxWidth);
void renderText(char *text, double fontSizeMultiplier, int x, int y);
void setFontColor(int r, int g, int b);

#endif
