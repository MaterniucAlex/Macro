#include <stdio.h>
#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "./textRenderLib/textRenderer.h"

void* safeCreate(void* ptr)
{
	if (ptr == NULL) printf("%s\n", SDL_GetError());
	return ptr;
}

int main() {

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window   *window   = SDL_CreateWindow  ("Macro", 400, 300, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

	SDL_Texture *fontAtlas = safeCreate(IMG_LoadTexture   (renderer, "img/fontAtlas.png"));

	initTextRenderer(renderer, fontAtlas);

	SDL_Event event;
	bool isRunning = true;
	while(isRunning)
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_EVENT_QUIT:
					isRunning = false;
					break;
			}
		}

		SDL_RenderClear(renderer);
		setTextWrapping(400);	
		renderText("AAA aaa kkk", 2, 0, 0);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}