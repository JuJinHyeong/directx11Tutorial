#include "MouseClass.h"

void MouseClass::OnLeftPressed(int x, int y) {
	mIsLeftDown = true;
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::LPress, x, y));
}

void MouseClass::OnLeftReleased(int x, int y) {
	mIsLeftDown = false;
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::LRelease, x, y));
}

void MouseClass::OnRightPressed(int x, int y) {
	mIsRightDown = true;
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::RPress, x, y));
}

void MouseClass::OnRightReleased(int x, int y) {
	mIsRightDown = false;
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::RRelease, x, y));
}

void MouseClass::OnMiddlePressed(int x, int y) {
	mIsMiddleDown = true;
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::MPress, x, y));
}

void MouseClass::OnMiddleReleased(int x, int y) {
	mIsMiddleDown = false;
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::MRelease, x, y));
}

void MouseClass::OnWheelUp(int x, int y) {
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::WheelUp, x, y));
}

void MouseClass::OnWheelDown(int x, int y) {
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::WheelDown, x, y));
}

bool MouseClass::IsLeftDown() {
	return mIsLeftDown;
}

bool MouseClass::IsMiddleDown() {
	return mIsMiddleDown;
}

bool MouseClass::IsRightDown() {
	return mIsRightDown;
}

void MouseClass::OnMouseMove(int x, int y) {
	mX = x;
	mY = y;
	mEventBuffer.push(MouseEvent(MouseEvent::EventType::Move, x, y));
}

void MouseClass::OnMouseMoveRaw(int x, int y) {
	this->mEventBuffer.push(MouseEvent(MouseEvent::EventType::RAW_MOVE, x, y));
}

MousePoint MouseClass::GetPos() {
	return { mX, mY };
}

int MouseClass::GetPosX() {
	return mX;
}

int MouseClass::GetPosY() {
	return mY;
}

bool MouseClass::EventBufferIsEmpty() {
	return mEventBuffer.empty();
}

MouseEvent MouseClass::ReadEvent() {
	if (mEventBuffer.empty()) {
		return MouseEvent();
	}
	else {
		MouseEvent e = mEventBuffer.front();
		mEventBuffer.pop();
		return e;
	}
}