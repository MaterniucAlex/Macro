#include "../Action.h"

#pragma once
#ifndef KEY_INTERACTION_LIB
#define KEY_INTERACTION_LIB

Action getCurrentAction();
void pressKey(char keyChar);
void releaseKey(char keyChar);

#endif