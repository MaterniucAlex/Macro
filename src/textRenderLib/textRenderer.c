#include "textRenderer.h"
#include <stdio.h>

SDL_Renderer *renderer;
SDL_Texture *fontAtlas;

SDL_FRect characterRect;

int pxWrap = 0;

void initTextRenderer(SDL_Renderer *rend, SDL_Texture *fontTexture)
{
	renderer = rend;
	fontAtlas = fontTexture;

	characterRect.w = 15;
	characterRect.h = 24;
}

void setTextWrapping(int maxWidth)
{
	pxWrap = maxWidth;
}

void renderText(char *text, double fontSize, int x, int y)
{
	int X = x;
	SDL_FRect drawingRect;
	drawingRect.x = x;
	drawingRect.y = y;
	drawingRect.h = characterRect.h * fontSize;
	drawingRect.w = characterRect.w * fontSize;

	int i = 0;

	while(text[i] != '\0')
	{
		characterRect.x = (text[i] - 32) % 32 * characterRect.w;
		characterRect.y = (text[i] - 32) / 32 * characterRect.h;

		if (characterRect.x < 0 || characterRect.y < 0 || characterRect.x > characterRect.w * 31 || characterRect.y > characterRect.h * 2)
		{
			characterRect.x = 0;
			characterRect.y = 0;
		}

		SDL_RenderTexture(renderer, fontAtlas, &characterRect, &drawingRect);
		drawingRect.x += drawingRect.w;

		if (pxWrap != 0)
		if (drawingRect.x + drawingRect.w > X + pxWrap)
		{
			drawingRect.x = X;
			drawingRect.y += drawingRect.h;
		}	
		i++;
	}
}