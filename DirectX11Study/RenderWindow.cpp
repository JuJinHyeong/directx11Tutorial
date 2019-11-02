#include "WindowContainer.h"

RenderWindow::RenderWindow()
{

}

RenderWindow::~RenderWindow() {
	if (mHandle != NULL) {
		UnregisterClass(mWindowClassWide.c_str(), mHInstance);
		DestroyWindow(mHandle);
	}
}

bool RenderWindow::Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, const std::string& windowTitle, const std::string& windowClass, int width, int height) {
	mHInstance = hInstance;
	mWidth = width;
	mHeight = height;
	mWindowTitle = windowTitle;
	mWindowTitleWide = StringConverter::StringToWide(windowTitle);
	mWindowClass = windowClass;
	mWindowClassWide = StringConverter::StringToWide(windowClass);

	RegisterWindowClass();

	int centerScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - mWidth / 2;
	int centerScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - mHeight / 2;

	RECT wr;
	wr.left = centerScreenX;
	wr.top = centerScreenY;
	wr.right = wr.left + mWidth;
	wr.bottom = wr.top + mHeight;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	mHandle = CreateWindowEx(0,
		mWindowClassWide.c_str(),
		mWindowTitleWide.c_str(),
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		wr.left,
		wr.top,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		mHInstance,
		pWindowContainer
	);

	if (mHandle == NULL) {
		ErrorLogger::Log(GetLastError(), "CreateWindowEX Failed for window: " + mWindowTitle);
		return false;
	}

	ShowWindow(mHandle, SW_SHOW);
	SetForegroundWindow(mHandle);
	SetFocus(mHandle);

	return true;
}

bool RenderWindow::ProcessMessages() {
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (PeekMessage(&msg, mHandle, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_NULL) {
		if (!IsWindow(mHandle)) {
			mHandle = NULL;
			UnregisterClass(mWindowClassWide.c_str(), mHInstance);
			return false;
		}
	}

	return true;
}

HWND RenderWindow::GetHWND() const {
	return mHandle;
}

LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
	default:
	{
		WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
}

LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_NCCREATE:
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		WindowContainer* pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);
		if (pWindow == nullptr) {
			ErrorLogger::Log("Critical Error: Pointer to window container is null during WM_NCREATE.");
			exit(-1);
		}
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

void RenderWindow::RegisterWindowClass() {
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = HandleMessageSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mHInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = mWindowClassWide.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&wc);
}