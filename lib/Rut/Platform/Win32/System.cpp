#include "System.h"

#include <stdio.h>
#include <Windows.h>


namespace Rut::Platform
{
	bool AllocConsole(const wchar_t* wpTile)
	{
		if (::AllocConsole() && ::AttachConsole(ATTACH_PARENT_PROCESS))
		{
			if (wpTile) { ::SetConsoleTitleW(wpTile); }
			return true;
		}
		return false;
	}

	bool PutConsole(const char* cpText, size_t nSize)
	{
		DWORD written = 0;
		return ::WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), cpText, (DWORD)nSize, &written, nullptr);
	}

	bool PutConsole(const wchar_t* wpText, size_t nSize)
	{
		DWORD written = 0;
		return ::WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), wpText, (DWORD)nSize, &written, nullptr);
	}


	size_t CreateThread(void(*fnThread)())
	{
		return (size_t)::CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)fnThread, nullptr, 0, nullptr);
	}
}