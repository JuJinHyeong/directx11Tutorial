#include "KeyboardEvent.h"

KeyboardEvent::KeyboardEvent() : 
	mType(EventType::Invaild),
	mKey(0u)
{
}

KeyboardEvent::KeyboardEvent(const EventType type, const unsigned char key) :
	mType(type),
	mKey(key)
{
}

bool KeyboardEvent::IsPressed() const {
	return mType == EventType::Press;
}

bool KeyboardEvent::IsReleased() const {
	return mType == EventType::Release;
}

bool KeyboardEvent::IsVaild() const {
	return mType != EventType::Invaild;
}

unsigned char KeyboardEvent::GetKeyCode() const {
	return mKey;
}