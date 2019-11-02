#pragma once
#include"ErrorLogger.h"

class WindowContainer;

class RenderWindow
{
public:
	bool Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, const std::string& windowTitle, const std::string& windowClass, int width, int height);
	bool ProcessMessages();
	HWND GetHWND() const;
	RenderWindow();
	~RenderWindow();
private:
	void RegisterWindowClass();
	HWND mHandle = NULL;
	HINSTANCE mHInstance = NULL;
	std::string mWindowTitle;
	std::wstring mWindowTitleWide;
	std::string mWindowClass;
	std::wstring mWindowClassWide;
	int mWidth = 0;
	int mHeight = 0;
};