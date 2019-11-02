#include "KeyboardClass.h"
#include <cstring>

KeyboardClass::KeyboardClass() {
	memset(mKeyStatus, 0, sizeof(mKeyStatus));
}

bool KeyboardClass::KeyIsPressed(const unsigned char keycode) {
	return mKeyStatus[keycode];
}

bool KeyboardClass::KeyBufferIsEmpty() {
	return mKeyBuffer.empty();
}

bool KeyboardClass::CharBufferIsEmpty() {
	return mCharBuffer.empty();
}

KeyboardEvent KeyboardClass::ReadKey() {
	if (mKeyBuffer.empty()) {
		return KeyboardEvent();
	}
	else {
		KeyboardEvent e = mKeyBuffer.front();
		mKeyBuffer.pop();
		return e;
	}
}

unsigned char KeyboardClass::ReadChar() {
	if (mCharBuffer.empty()) {
		return 0u;
	}
	else {
		unsigned char e = mCharBuffer.front();
		mCharBuffer.pop();
		return e;
	}
}

void KeyboardClass::OnKeyPressed(const unsigned char key) {
	mKeyStatus[key] = true;
	mKeyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Press, key));
}

void KeyboardClass::OnKeyReleased(const unsigned char key) {
	mKeyStatus[key] = false;
	mKeyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Release, key));
}

void KeyboardClass::OnChar(const unsigned char key) {
	mCharBuffer.push(key);
}

void KeyboardClass::EnableAutoRepeatKeys() {
	mAutoRepeatKeys = true;
}

void KeyboardClass::DisableAutoRepeatKeys() {
	mAutoRepeatKeys = false;
}

void KeyboardClass::EnableAutoRepeatChars() {
	mAutoRepeatChars = true;
}

void KeyboardClass::DisableAutoRepeatChars() {
	mAutoRepeatChars = false;
}

bool KeyboardClass::IsKeyAutoRepeat() {
	return mAutoRepeatKeys;
}

bool KeyboardClass::IsCharAutoRepeat() {
	return mAutoRepeatChars;
}
