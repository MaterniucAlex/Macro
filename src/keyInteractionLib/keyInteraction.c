#include "SDL3/SDL.h"
#include "keyInteraction.h"
#include <stdio.h>
#include <windows.h>
#include <winscard.h>

long timeSinceLastAction = 0;

bool prevKeysPressed[256] = {false};

Action getCurrentAction() {

    Action currentAction;
    currentAction.key = -1;
    currentAction.mouseX = -1;
    currentAction.mouseY = -1;
    currentAction.timeDelay = 0;
    currentAction.state = RELEASED;

    for (int key = 0; key <= 255; key++) {

        bool isPressed = (GetAsyncKeyState(key) & 0x8000) != 0;

        if (prevKeysPressed[key] == isPressed) continue;

        prevKeysPressed[key] = isPressed;

        currentAction.key = key;
        currentAction.state = isPressed ? PRESSED : RELEASED;
        currentAction.timeDelay = SDL_GetTicks() - timeSinceLastAction;

        if (key <= 0x06 && key != 0x03) 
        {
           POINT cursorPos;
           GetCursorPos(&cursorPos);
           currentAction.mouseX = cursorPos.x;
           currentAction.mouseY = cursorPos.y;
        }

        timeSinceLastAction = SDL_GetTicks();
        break;
    }

    return currentAction;
}

void pressKey(char key) {

    INPUT ip = {0};

    if (key <= 0x06 && key != 0x03) // All mouse buttons | 0x03 is PauseBreak
    {
        ip.type = INPUT_MOUSE;
        
        switch (key) {
        case 0x01: //Left mouse button
            ip.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
            break;
        case 0x02: //Right mouse button
            ip.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
            break;
        case 0x04: //Middle mouse button
            ip.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
            break;
        case 0x05: //Forward mouse button
            ip.mi.dwFlags = MOUSEEVENTF_XDOWN;
            ip.mi.mouseData = XBUTTON1;
            break;
        case 0x06: //Back mouse button
            ip.mi.dwFlags = MOUSEEVENTF_XDOWN;
            ip.mi.mouseData = XBUTTON2;
            break;
        }
    }
    else
    {
        ip.type = INPUT_KEYBOARD;
        ip.ki.wVk = key;
    }

    SendInput(1, &ip, sizeof(INPUT));
}

void releaseKey(char key) {
    INPUT ip = {0};

    if (key <= 0x06 && key != 0x03) // All mouse buttons | 0x03 is PauseBreak
    {
        ip.type = INPUT_MOUSE;
        switch (key) {
        case 0x01: //Left mouse button
            ip.mi.dwFlags = MOUSEEVENTF_LEFTUP;
            break;
        case 0x02: //Right mouse button
            ip.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
            break;
        case 0x04: //Middle mouse button
            ip.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
            break;
        case 0x05: //Forward mouse button
            ip.mi.dwFlags = MOUSEEVENTF_XUP;
            ip.mi.mouseData = XBUTTON1;
            break;
        case 0x06: //Back mouse button
            ip.mi.dwFlags = MOUSEEVENTF_XUP;
            ip.mi.mouseData = XBUTTON2;
            break;
        }
    }
    else
    {
        ip.type = INPUT_KEYBOARD;
        ip.ki.wVk = key;
    }

    SendInput(1, &ip, sizeof(INPUT));
}