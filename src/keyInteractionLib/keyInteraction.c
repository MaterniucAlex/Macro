#include "SDL3/SDL.h"
#include "keyInteraction.h"
#include <stdio.h>
#include <windows.h>

long timeSinceLastAction = 0;

bool prevKeysPressed[256] = {false};

Action getCurrentAction() {

    Action currentAction;
    currentAction.key = 0;

    for (int key = 8; key <= 255; key++) {

        bool isPressed = (GetAsyncKeyState(key) & 0x8000) != 0;

        if (prevKeysPressed[key] == isPressed) continue;

        prevKeysPressed[key] = isPressed;

        currentAction.key = key;
        currentAction.state = isPressed ? PRESSED : RELEASED;
        currentAction.timeDelay = SDL_GetTicks() - timeSinceLastAction;

        timeSinceLastAction = SDL_GetTicks();
        break;
    }

    return currentAction;
}

void pressKey(char key) {
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
    ip.ki.wVk = key;
    ip.ki.dwFlags = 0;
    SendInput(1, &ip, sizeof(INPUT));
}

void releaseKey(char key) {
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
    ip.ki.wVk = key;
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));
}
