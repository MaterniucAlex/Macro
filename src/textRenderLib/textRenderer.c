#include "textRenderer.h"

SDL_Renderer *renderer;
SDL_Texture *fontAtlas;

SDL_FRect characterRect;

void initTextRenderer(SDL_Renderer *rend, SDL_Texture *fontTexture)
{
	renderer = rend;
	fontAtlas = fontTexture;

	characterRect.w = 20;
	characterRect.h = 40;
}

void renderText(char *text, int fontSize, int x, int y)
{
	SDL_FRect drawingRect;
	drawingRect.x = x;
	drawingRect.y = y;
	drawingRect.h = fontSize;
	drawingRect.w = fontSize / 2;

	int i = 0;

	while(text[i] != '\0')
	{
		characterRect.x = (text[i] - 32) % 32 * characterRect.w;
		characterRect.y = (text[i] - 32) / 32 * characterRect.h;

		if (characterRect.x < 0 || characterRect.y < 0)
		{
			characterRect.x = 0;
			characterRect.y = 0;
		}

		SDL_RenderTexture(renderer, fontAtlas, &characterRect, &drawingRect);
		drawingRect.x += drawingRect.w;
		i++;
	}
}

void renderTextWrapped(char *text, int fontSize, int x, int y, int pxWrap)
{
	int X = x;
	SDL_FRect drawingRect;
	drawingRect.x = x;
	drawingRect.y = y;
	drawingRect.h = fontSize;
	drawingRect.w = drawingRect.h / 2;

	int i = 0;

	while(text[i] != '\0')
	{
		characterRect.x = (text[i] - 32) % 32 * characterRect.w;
		characterRect.y = (text[i] - 32) / 32 * characterRect.h;

		if (characterRect.x < 0 || characterRect.y < 0)
		{
			characterRect.x = 0;
			characterRect.y = 0;
		}

		SDL_RenderTexture(renderer, fontAtlas, &characterRect, &drawingRect);
		drawingRect.x += drawingRect.w;

		if (drawingRect.x + drawingRect.w > X + pxWrap)
		{
			drawingRect.x = X;
			drawingRect.y += drawingRect.h;
		}	
		i++;
	}
}