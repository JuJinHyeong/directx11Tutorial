#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, const std::string& windowTitle, const std::string& windowClass, int width, int height) {
	mTimer.Start();
	return mRenderWindow.Initialize(this, hInstance, windowTitle, windowClass, width, height)
		&& gfx.Initialize(mRenderWindow.GetHWND(), width, height);
}

bool Engine::ProcessMessages() {
	return mRenderWindow.ProcessMessages();
}

void Engine::Update() {
	float deltaTime = mTimer.GetMilesecondsElapsed();
	mTimer.Restart();

	while (!keyboard.CharBufferIsEmpty()) {
		unsigned char ch = keyboard.ReadChar();
	}
	while (!keyboard.KeyBufferIsEmpty()) {
		KeyboardEvent e = keyboard.ReadKey();
		unsigned char keycode = e.GetKeyCode();
	}
	while (!mouse.EventBufferIsEmpty()) {
		MouseEvent me = mouse.ReadEvent();
		if (me.GetType() == MouseEvent::EventType::WheelUp) {
			OutputDebugStringA("MouseWheelUp\n");
		}
		if (me.GetType() == MouseEvent::EventType::WheelDown) {
			OutputDebugStringA("MouseWheelDown\n");
		}
		if (mouse.IsRightDown()) {
			if (me.GetType() == MouseEvent::EventType::RAW_MOVE) {
				gfx.camera.AdjustRotation(static_cast<float>(me.GetPosY()) * 0.01f, static_cast<float>(me.GetPosX()) * 0.01f, 0);
			}
		}
	}

	gfx.gameObject.AdjustRotation(0.0f, 0.001f * deltaTime, 0.0f);
	
	const float cameraSpeed = 0.005f;

	if (keyboard.KeyIsPressed('W')) {
		gfx.camera.AdjustPosition(gfx.camera.GetForwardVector() * cameraSpeed * deltaTime);
	}
	if (keyboard.KeyIsPressed('S')) {
		gfx.camera.AdjustPosition(gfx.camera.GetBackwardVector() * cameraSpeed * deltaTime);
	}
	if (keyboard.KeyIsPressed('A')) {
		gfx.camera.AdjustPosition(gfx.camera.GetLeftVector() * cameraSpeed * deltaTime);
	}
	if (keyboard.KeyIsPressed('D')) {
		gfx.camera.AdjustPosition(gfx.camera.GetRightVector() * cameraSpeed * deltaTime);
	}
	if (keyboard.KeyIsPressed(VK_SPACE)) {
		gfx.camera.AdjustPosition(0.0f, cameraSpeed * deltaTime, 0.0f);
	}
	if (keyboard.KeyIsPressed('Z')) {
		gfx.camera.AdjustPosition(0.0f, -cameraSpeed * deltaTime, 0.0f);
	}
}

void Engine::RenderFrame() {
	gfx.RenderFrame();
}