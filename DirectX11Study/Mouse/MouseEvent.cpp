#include "MouseEvent.h"

MouseEvent::MouseEvent() :
	mType(EventType::Invalid),
	mX(0),
	mY(0)
{

}

MouseEvent::MouseEvent(EventType type, int x, int y) :
	mType(type),
	mX(x),
	mY(y)
{

}

bool MouseEvent::IsValid() const {
	return mType != EventType::Invalid;
}

MouseEvent::EventType MouseEvent::GetType() const {
	return mType;
}

MousePoint MouseEvent::GetPos() const {
	return { mX, mY };
}

int MouseEvent::GetPosX() const {
	return mX;
}

int MouseEvent::GetPosY() const {
	return mY;
}