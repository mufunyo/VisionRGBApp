#define WIN32_LEAN_AND_MEAN
#include "stdafx.h"
#include <d3d9.h>
#include <dxva2api.h>
#include <strsafe.h>
#include "Error.hpp"

CStatusBar* g_statusBar = nullptr;

void crash(LPCTSTR fmt, ...) {
	TCHAR str[256];
	va_list ap;
	va_start(ap, fmt);
	StringCchVPrintf(str, 256, fmt, ap);
	MessageBox(NULL, str, TEXT("Error!"), MB_OK | MB_ICONERROR);
#ifdef _DEBUG
	DebugBreak();
#endif
	ExitProcess(FALSE);
}

void warning(LPCTSTR fmt, ...) {
	TCHAR str[256];
	va_list ap;
	va_start(ap, fmt);
	StringCchVPrintf(str, 256, fmt, ap);
	MessageBox(NULL, str, TEXT("Warning!"), MB_OK | MB_ICONWARNING);
	va_end(ap);
}

void notice(LPCTSTR fmt, ...) {
	TCHAR str[256];
	va_list ap;
	va_start(ap, fmt);
	StringCchVPrintf(str, 256, fmt, ap);
	TCHAR str2[256];
	StringCchPrintf(str2, 256, TEXT("%s\n"), str);
#ifdef _DEBUG
	OutputDebugString(str2);
#endif
	if (g_statusBar)
		g_statusBar->SetPartText(0, str2);
}

bool hrAssert(HRESULT hr, AssertLvl level, LPCTSTR caller) {
	if (SUCCEEDED(hr))
		return false;
	else {
		LPTSTR str = nullptr;
		if (!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&str, 0, nullptr))
			warning(TEXT("%s: Unknown error code, result = 0x%08lX"), caller, hr);
		else {
			switch (level) {
#ifdef _DEBUG
			case ASSERT_DEBUG:
#endif
			case ASSERT_NOTICE:
				notice(TEXT("%s: %s"), caller, str);
				break;
			case ASSERT_WARNING:
				warning(TEXT("%s: %s"), caller, str);
				break;
			case ASSERT_CRASH:
				crash(TEXT("%s: %s"), caller, str);
			}
		}
		return true;
	}
}