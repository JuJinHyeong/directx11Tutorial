#pragma once

#include "KeyboardEvent.h"
#include <queue>

class KeyboardClass {
public:
	KeyboardClass();
	bool KeyIsPressed(const unsigned char keycode);
	bool KeyBufferIsEmpty();
	bool CharBufferIsEmpty();
	KeyboardEvent ReadKey();
	unsigned char ReadChar();
	void OnKeyPressed(const unsigned char key);
	void OnKeyReleased(const unsigned char key);
	void OnChar(const unsigned char key);
	void EnableAutoRepeatKeys();
	void DisableAutoRepeatKeys();
	void EnableAutoRepeatChars();
	void DisableAutoRepeatChars();
	bool IsKeyAutoRepeat();
	bool IsCharAutoRepeat();

private:
	bool mAutoRepeatKeys = false;
	bool mAutoRepeatChars = false;
	bool mKeyStatus[256];
	std::queue<KeyboardEvent> mKeyBuffer;
	std::queue<unsigned char> mCharBuffer;
};