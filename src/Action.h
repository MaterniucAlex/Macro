#pragma once
#ifndef ACTION
#define ACTION

enum KeyState {
	PRESSED,
	RELEASED
};

typedef struct {
	char key;
	int timeDelay;
	enum KeyState state;
	int mouseX;
	int mouseY;
} Action;

#endif