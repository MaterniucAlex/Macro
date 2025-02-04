#include <stdio.h>
#include <synchapi.h>
#include <windows.h>
#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "./textRenderLib/textRenderer.h"
#include "./keyInteractionLib/keyInteraction.h"
#include "Action.h"

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

	Action lastAction = getCurrentAction();
	Action actionList[20];
	int nextActionListIndex = 0;

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

		Action currentAction = getCurrentAction();
		if (currentAction.character == 8 && currentAction.state == RELEASED) //backspace
		{
			for (int i = 1; i < 20; i++) 
			{
				if (actionList[i].state == PRESSED)
					pressKey(actionList[i].character);
				else
					releaseKey(actionList[i].character);
				SDL_Delay(actionList[i].timeDelay);
			}
			lastAction = currentAction;
		}
		if((lastAction.character != currentAction.character ||
			lastAction.state 	!= currentAction.state) && currentAction.character != 0)
		{
			lastAction = currentAction;
			printf("%c::%d::%d\n", lastAction.character, lastAction.state == PRESSED, lastAction.timeDelay);
			actionList[nextActionListIndex] = lastAction;
			nextActionListIndex += nextActionListIndex == 20 ? -20 : 1;
		}

		char text[20];
		for(int i = 0; i < 20; i++) text[i] = actionList[i].character;

		SDL_RenderClear(renderer);
		renderText(text, 2, 0, 0);
		SDL_RenderPresent(renderer);
		SDL_Delay(1);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}