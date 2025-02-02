#include "keyInteraction.h"
#include <stdio.h>
#include <windows.h>

char getCurrentKeyPressed() {
    for (int key = 8; key <= 255; key++) { // Loop through possible key codes
        if (GetAsyncKeyState(key) & 0x8000) { // Check if key is down
            // Convert virtual key code to character
            BYTE keyboardState[256];
            GetKeyboardState(keyboardState);

            WORD charCode;
            if (ToAscii((UINT)key, MapVirtualKey(key, MAPVK_VK_TO_VSC), keyboardState, &charCode, 0) == 1) {
                return (char)charCode;
            }
        }
    }
    return 0; // No key pressed
}