#include "../Action.h"

#pragma once
#ifndef KEY_INTERACTION_LIB
#define KEY_INTERACTION_LIB

Action getCurrentAction();
void pressKey(char key);
void releaseKey(char key);

#endif