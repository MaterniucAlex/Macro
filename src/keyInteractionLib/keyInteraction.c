#include "SDL3/SDL.h"
#include "keyInteraction.h"
#include <stdio.h>
#include <windows.h>

long timeSinceLastAction = 0;

bool prevKeysPressed[256] = {false};

Action getCurrentAction() {

    Action currentAction;
    currentAction.character = 0;

    for (int key = 8; key <= 255; key++) {

        bool isPressed = (GetAsyncKeyState(key) & 0x8000) != 0;

        if (prevKeysPressed[key] == isPressed) continue;

        prevKeysPressed[key] = isPressed;

        currentAction.state = isPressed ? PRESSED : RELEASED;

        BYTE keyboardState[256];
        GetKeyboardState(keyboardState);
        WORD charCode;
        if (ToAscii((UINT)key, MapVirtualKey(key, MAPVK_VK_TO_VSC), keyboardState, &charCode, 0) == 1) {
            currentAction.character = charCode;
        }

        currentAction.timeDelay = SDL_GetTicks() - timeSinceLastAction;
        timeSinceLastAction = SDL_GetTicks();
        break;
    }

    return currentAction; // No key pressed
}

void pressKey(char keyChar) {
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
    ip.ki.wVk = VkKeyScan(keyChar);
    ip.ki.dwFlags = 0;
    SendInput(1, &ip, sizeof(INPUT));
}

void releaseKey(char keyChar) {
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
    ip.ki.wVk = VkKeyScan(keyChar);
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));
}
