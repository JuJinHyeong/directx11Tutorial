#pragma once
#include "COMExecption.h"
#include<Windows.h>

class ErrorLogger
{
public:
	static void Log(const std::string& msg);
	static void Log(HRESULT hr, const std::string& msg);
	static void Log(HRESULT hr, const std::wstring& msg);
	static void Log(COMException& exception);
};