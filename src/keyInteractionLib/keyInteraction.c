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
// WORD char_to_vk(char c) {
//     return VkKeyScan(c) & 0xFF; // Extract the virtual key code
// }

// WORD char_to_scancode(char c) {
//     SHORT vk = VkKeyScanA(c);  // Get the virtual key code
//     if (vk == -1) return 0;    // Character has no virtual key mapping

//     WORD vkey = vk & 0xFF;     // Extract virtual key code
//     return (WORD)MapVirtualKey(vkey, MAPVK_VK_TO_VSC);  // Convert to scan code
// }

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
