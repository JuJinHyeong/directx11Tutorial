#pragma once

class KeyboardEvent {
public:
	enum EventType {
		Press,
		Release,
		Invaild
	};

	KeyboardEvent();
	KeyboardEvent(const EventType type, const unsigned char key);
	bool IsPressed() const;
	bool IsReleased() const;
	bool IsVaild() const;
	unsigned char GetKeyCode() const;

private:
	EventType mType;
	unsigned char mKey;
};