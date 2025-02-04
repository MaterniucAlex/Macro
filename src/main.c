#include <stdio.h>
#include <synchapi.h>
#include <windows.h>
#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "./textRenderLib/textRenderer.h"
#include "./keyInteractionLib/keyInteraction.h"

void* safeCreate(void* ptr)
{
	if (ptr == NULL) printf("%s\n", SDL_GetError());
	return ptr;
}

int main() {

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window   *window   = safeCreate(SDL_CreateWindow  ("Macro", 400, 300, 0));
	SDL_Renderer *renderer = safeCreate(SDL_CreateRenderer(window, NULL));

	SDL_Texture *fontAtlas = safeCreate(IMG_LoadTexture   (renderer, "img/fontAtlas.png"));

	initTextRenderer(renderer, fontAtlas);
	setTextWrapping(400);	

	char lastKeyPressed = ' ';
	char charList[20] = {' '};

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

		char key = getCurrentKeyPressed();
		if (key == 8) //backspace
		{
			for (int i = 1; i < 20; i++) 
			{
				pressKey(charList[i]);
				printf("%c ", charList[i]);
				SDL_Delay(100);
				releaseKey(charList[i]);
			}
		}
		if(lastKeyPressed != key && key != 0)
		{
			lastKeyPressed = key;
			for(int i = 18; i >= 0; i--)
				charList[i+1] = charList[i];
			charList[0] = lastKeyPressed;
		}


		SDL_RenderClear(renderer);
		renderText(charList, 2, 0, 0);
		SDL_RenderPresent(renderer);
		SDL_Delay(1);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}