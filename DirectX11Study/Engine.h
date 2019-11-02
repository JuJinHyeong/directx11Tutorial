#pragma once
#include "WindowContainer.h"
#include "Timer.h"
class Engine : WindowContainer {
private:
	Timer mTimer;
public:
	bool Initialize(HINSTANCE hInstance, const std::string& windowTitle, const std::string& windowClass, int width, int height);
	bool ProcessMessages();
	void Update();
	void RenderFrame();
};