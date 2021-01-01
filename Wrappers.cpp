#include "stdafx.h"
#include "Wrappers.hpp"
#include "d3dx9.h"

Mutex::Mutex() {
	InitializeCriticalSection(&data);
}

void Mutex::lock() {
	EnterCriticalSection(&data);
}

void Mutex::unlock() {
	LeaveCriticalSection(&data);
}