#pragma once

class Win32xx::CStatusBar;
extern CStatusBar *g_statusBar;

enum AssertLvl {
	ASSERT_DEBUG,
	ASSERT_NOTICE,
	ASSERT_WARNING,
	ASSERT_CRASH,
};

void crash(LPCTSTR fmt, ...);
void warning(LPCTSTR fmt, ...);
void notice(LPCTSTR fmt, ...);
bool hrAssert(HRESULT hr, AssertLvl level, LPCTSTR caller = TEXT("hrAssert"));
