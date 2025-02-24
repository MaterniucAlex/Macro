#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <unistd.h>
#include <pthread.h>
#include <winscard.h>

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"

#include "./textRenderLib/textRenderer.h"
#include "./keyInteractionLib/keyInteraction.h"
#include "Action.h"

/*
	TODO:
	-Add mouse scroll & dragging
	-Add UI
	-Add custom loading for scripts
*/

void* safeCreate(void* ptr)
{
	if (ptr == NULL) printf("%s\n", SDL_GetError());
	return ptr;
}
void *pressKeysFunction(void* args);
char  VirtualKeyToChar (UINT virtualKey, bool shiftPressed);

bool hasKeyPressThreadEnded = true;

#define bodyTextSize (14 * 5)
char headingText[14] = "F2 - Record";
char bodyText[bodyTextSize] = "F1 - Play";
char bodyTextOverlay[bodyTextSize] = "";
int lastCharID = 0;

int main() {

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window   *window   = safeCreate(SDL_CreateWindow  ("Macro", 400, 300, 0));
	SDL_Renderer *renderer = safeCreate(SDL_CreateRenderer(window, NULL));

	SDL_Texture *fontAtlas = safeCreate(IMG_LoadTexture   (renderer, "img/fontAtlas.png"));

	initTextRenderer(renderer, fontAtlas);
	setTextWrapping(400);	

	char *saveFileName = "lastMacro.mkr";
	FILE *saveFile = NULL;

	bool isRecording = false;
	pthread_t keyPressThread;

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
		if (currentAction.key != -1)
		switch (currentAction.key) {
			case VK_F1:
				if (currentAction.state != RELEASED) break;
				if (!hasKeyPressThreadEnded)
				{
					pthread_cancel(keyPressThread);
					break;
				}
				if (isRecording)
				{
					isRecording = false;
					fclose(saveFile);
				}
				printf("started running\n");
				strcpy(headingText, "Running");
				hasKeyPressThreadEnded = false;
				pthread_create(&keyPressThread, NULL, pressKeysFunction, saveFileName);
				break;
			case VK_F2:
				if (currentAction.state != RELEASED || !hasKeyPressThreadEnded) break;
				isRecording = !isRecording;
				if (isRecording)
				{
					saveFile = fopen(saveFileName, "w+");
					printf("Started Recording\n");
					strcpy(headingText, "Recording");
				}
				else
				{
					fclose(saveFile);
					printf("Stopped Recording\n");
					strcpy(headingText, "");
				}
				break;
			default:
				if (!isRecording || saveFile == NULL) break;
				printf("Printing: %d %c %d %d %d\n",
					currentAction.key, currentAction.state == PRESSED ? 'P' : 'R', currentAction.timeDelay, currentAction.mouseX, currentAction.mouseY);

				fprintf(saveFile, "%d %c %d %d %d\n",
					currentAction.key, currentAction.state == PRESSED ? 'P' : 'R', currentAction.timeDelay, currentAction.mouseX, currentAction.mouseY);

				char actionChar = VirtualKeyToChar(currentAction.key, VK_SHIFT & 0x8000);
				if (currentAction.state == PRESSED) 
				{
					bodyText[lastCharID] = actionChar;
					bodyTextOverlay[lastCharID] = ' ';
				}
				else 
				{
					bodyText[lastCharID] = ' ';
					bodyTextOverlay[lastCharID] = actionChar;
				}
				lastCharID++;
				if (lastCharID >= bodyTextSize) lastCharID = 0;
		}

		SDL_RenderClear(renderer);
		
		setFontColor(255, 255, 255);
		renderText(headingText, 2, 0, 0);

		setFontColor(200, 200, 200);
		renderText(bodyText, 2, 0, 48);

		setFontColor(200, 100, 100);
		renderText(bodyTextOverlay, 2, 0, 48);

		SDL_RenderPresent(renderer);
		SDL_Delay(1); //ms
	}

	if (saveFile != NULL) fclose(saveFile);

	closeTextRenderer();

	SDL_DestroyTexture(fontAtlas);
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
	int mouseX;
	int mouseY;

	while (fscanf(saveFile, "%d %c %d %d %d", &key, &state, &timeDelay, &mouseX, &mouseY) == 5) // 5 = nr of values read
	{
	
		if (key == -1) break;

		printf("Waiting %d | ", timeDelay);
		if (timeDelay > 0) usleep(timeDelay * 1000); //convert from microseconds to ms

		if (mouseX != -1 && mouseY != -1) SetCursorPos(mouseX, mouseY);

		printf("Key: %d\n", key);
		state == 'P' ? pressKey(key) : releaseKey(key);
	}

	hasKeyPressThreadEnded = true;
	fclose(saveFile);
	printf("stopped running\n");
	strcpy(headingText, "");
}

char VirtualKeyToChar(UINT virtualKey, bool shiftPressed) {

    HKL keyboardLayout = GetKeyboardLayout(0);
    UINT scanCode = MapVirtualKeyEx(virtualKey, MAPVK_VK_TO_VSC, keyboardLayout);

    BYTE keyboardState[256];
    GetKeyboardState(keyboardState);

    if (shiftPressed) keyboardState[VK_SHIFT] = 0x80;
    else 			  keyboardState[VK_SHIFT] = 0;

    wchar_t buffer[10] = {0};
    int result = ToUnicodeEx(virtualKey, scanCode, keyboardState, buffer, 10, 0, keyboardLayout);

    if (result > 0) return (char)buffer[0];

    return 0;
}
