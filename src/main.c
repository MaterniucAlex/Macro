#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <unistd.h>
#include <pthread.h>

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
void *pressKeysFunction(void* args);

bool hasKeyPressThreadEnded = true;

int main() {

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window   *window   = safeCreate(SDL_CreateWindow  ("Macro", 400, 300, 0));
	SDL_Renderer *renderer = safeCreate(SDL_CreateRenderer(window, NULL));

	SDL_Texture *fontAtlas = safeCreate(IMG_LoadTexture   (renderer, "img/fontAtlas.png"));

	initTextRenderer(renderer, fontAtlas);
	setTextWrapping(400);	

	char *saveFileName = "lastMacro.mkr";
	FILE *saveFile;

	bool isRecording = false;

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
		if (currentAction.key != 0)
		switch (currentAction.key) {
			case VK_F1:
				if (!hasKeyPressThreadEnded) break;
				printf("started running\n");
				hasKeyPressThreadEnded = false;
				pthread_t thread_id;
				pthread_create(&thread_id, NULL, pressKeysFunction, saveFileName);
				break;
			case VK_F2:
				if (currentAction.state != RELEASED) break;
				isRecording = !isRecording;
				if (isRecording)
				{
					saveFile = fopen(saveFileName, "w+");
					printf("Started Recording\n");
				}
				else
				{
					fclose(saveFile);
					printf("Stopped Recording\n");
				}
				break;
			default:
				if (!isRecording || saveFile == NULL) break;
				printf("Printing: %d %c %d\n",
					currentAction.key, currentAction.state == PRESSED ? 'P' : 'R', currentAction.timeDelay);

				fprintf(saveFile, "%d %c %d\n",
					currentAction.key, currentAction.state == PRESSED ? 'P' : 'R', currentAction.timeDelay);
		}

		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
		SDL_Delay(1);
	}

	fclose(saveFile);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}

void *pressKeysFunction(void* args)
{

	char *saveFileName = (char*) args;
	FILE *saveFile = fopen(saveFileName, "r");
	if (saveFile == NULL)
	{
		printf("Could not open file\n");
		return NULL;
	}

	int key;
	char state;
	int timeDelay;

	while (fscanf(saveFile, "%d %c %d", &key, &state, &timeDelay) == 3)
	{
	
		if (key == 0) break;

		printf("Waiting %d | ", timeDelay);
		if (timeDelay > 0) usleep(timeDelay * 1000); //convert from microseconds to ms

		printf("Key: %d\n", key);
		state == 'P' ? pressKey(key) : releaseKey(key);
	}

	hasKeyPressThreadEnded = true;
	fclose(saveFile);
	printf("stopped running\n");
}