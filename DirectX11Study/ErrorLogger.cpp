#include "ErrorLogger.h"
#include<comdef.h>

void ErrorLogger::Log(const std::string& message) {
	std::string errorMessage = "Error: " + message;
	MessageBoxA(NULL, errorMessage.c_str(), "Error", MB_ICONERROR);
}

void ErrorLogger::Log(HRESULT hr, const std::string& message) {
	_com_error error(hr);
	std::wstring errorMessage = L"Error: " + StringConverter::StringToWide(message) + L"\n"
		+ error.ErrorMessage();
	MessageBoxW(NULL, errorMessage.c_str(), L"Error", MB_ICONERROR);
}

void ErrorLogger::Log(HRESULT hr, const std::wstring& msg) {
	_com_error error(hr);
	std::wstring errorMessage = L"Error: " + msg + L"\n"
		+ error.ErrorMessage();
	MessageBoxW(NULL, errorMessage.c_str(), L"Error", MB_ICONERROR);
}

void ErrorLogger::Log(COMException& exception){
	std::wstring error_message = exception.what();
	MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}
