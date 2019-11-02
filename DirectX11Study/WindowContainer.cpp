#include "WindowContainer.h"
#include <memory>

WindowContainer::WindowContainer() {
	static bool isRawInputInitalized = false;
	if (!isRawInputInitalized) {
		RAWINPUTDEVICE rid;

		rid.usUsagePage = 0x01;
		rid.usUsage = 0x02;
		rid.dwFlags = 0;
		rid.hwndTarget = NULL;

		if (!RegisterRawInputDevices(&rid, 1, sizeof(rid))) {
			ErrorLogger::Log(GetLastError(), "Failed to register raw input device");
			exit(-1);
		}

		isRawInputInitalized = true;
	}
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam)) {
		return true;
	}
	
	switch (uMsg) {
	//keyboard
	case WM_KEYDOWN:
	{
		unsigned char keycode = static_cast<unsigned char>(wParam);
		if (keyboard.IsKeyAutoRepeat()) {
			keyboard.OnKeyPressed(keycode);
		}
		else {
			const bool wasPressed = lParam & 0x40000000;
			if (!wasPressed) {
				keyboard.OnKeyPressed(keycode);
			}
		}
		return 0;
	}
	case WM_KEYUP:
	{	
		unsigned char keycode = static_cast<unsigned char>(wParam);
		keyboard.OnKeyReleased(keycode);
		return 0;
	}
	case WM_CHAR:
	{
		unsigned char ch = static_cast<unsigned char>(wParam);
		if (keyboard.IsCharAutoRepeat()) {
			keyboard.OnChar(ch);
		}
		else {
			const bool wasPressed = lParam & 0x40000000;
			if (!wasPressed) {
				keyboard.OnChar(ch);
			}
		}
		return 0;
	}
	//Mouse messages
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnMouseMove(x, y);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnLeftPressed(x, y);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnRightPressed(x, y);
		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnMiddlePressed(x, y);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnLeftReleased(x, y);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnRightReleased(x, y);
		return 0;
	}
	case WM_MBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnMiddleReleased(x, y);
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) {
			mouse.OnWheelUp(x, y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0) {
			mouse.OnWheelDown(x, y);
		}
		break;
	}
	case WM_INPUT:
	{
		UINT dataSize = 0;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));
		if (dataSize > 0) {
			std::unique_ptr<BYTE[]> rawData = std::make_unique<BYTE[]>(dataSize);
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawData.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize) {
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawData.get());
				if (raw->header.dwType == RIM_TYPEMOUSE) {
					mouse.OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				}
			}
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}