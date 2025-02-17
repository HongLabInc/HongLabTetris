#pragma once

#include <Windows.h>

class ConsoleWin32
{
public:
	ConsoleWin32();
	virtual ~ConsoleWin32();

protected:
	HANDLE mConsoleOutput;
};
