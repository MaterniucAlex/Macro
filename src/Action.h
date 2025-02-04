#pragma once
#ifndef ACTION
#define ACTION

enum KeyState {
	PRESSED,
	RELEASED
};

typedef struct {
	char character;
	int timeDelay;
	enum KeyState state;
} Action;

#endif