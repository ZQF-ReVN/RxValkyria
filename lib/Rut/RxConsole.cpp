#include "RxConsole.h"
#include "RxStr.h"
#include "Platform/Platform.h"

#include <string>
#include <cstdio>
#include <locale.h>
#include <Windows.h>


namespace Rut::RxConsole
{
	static void SetConsoleNoQuickEdit()
	{
		DWORD mode = 0;
		::GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);
		::SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode & ~ENABLE_QUICK_EDIT_MODE);
	}

	static void SetConsoleLocale()
	{
		//system("chcp 65001");
		//system("chcp 936");
		setlocale(LC_ALL, "chs");
		//std::locale::global(std::locale(""));
	}

	static FILE* SetConsoleSTDIO()
	{
		FILE* fp_console = nullptr;
		freopen_s(&fp_console, "CONIN$", "r+t", stdin);
		freopen_s(&fp_console, "CONOUT$", "w+t", stdout);
		return fp_console;
	}


	FILE* Alloc(const wchar_t* lpTitle, bool isEdit)
	{
		Platform::AllocConsole(lpTitle);
		SetConsoleLocale();
		isEdit == false ? SetConsoleNoQuickEdit() : (void)(0);
		return SetConsoleSTDIO();
	}
}


namespace Rut::RxConsole
{
	static constexpr size_t sg_uiBufferCount = 1024;


	bool Put(const char* cpStr)
	{
		return Put(cpStr, strlen(cpStr));
	}

	bool Put(const char* cpStr, size_t nChar)
	{
		return Platform::PutConsole(cpStr, nChar);
	}

	bool Put(std::string_view msStr)
	{
		return Put(msStr.data(), msStr.size());
	}

	bool Put(const wchar_t* wpStr)
	{
		return Put(wpStr, wcslen(wpStr));
	}

	bool Put(const wchar_t* wpStr, size_t nChar)
	{
		return Platform::PutConsole(wpStr, nChar);
	}

	bool Put(std::wstring_view wsStr)
	{
		return Put(wsStr.data(), wsStr.size());
	}

	bool PutMBCS(const char* cpStr, size_t nCodePage)
	{
		std::wstring out_str;
		size_t cch = RxStr::ToWCS(cpStr, out_str, nCodePage);
		return Put(out_str.c_str(), cch);
	}

	bool PutMBCS(std::string_view msStr, size_t nCodePage)
	{
		return PutMBCS(msStr.data(), nCodePage);
	}

	bool PutFormat(const char* cpFormat, ...)
	{
		char buffer[sg_uiBufferCount];

		va_list args = nullptr;
		va_start(args, cpFormat);
		size_t cch = Platform::Sprintf_V(buffer, sg_uiBufferCount, cpFormat, args);
		va_end(args);

		return (cch <= 0) ? (false) : (Put(buffer, cch));
	}

	bool PutFormat(const wchar_t* cpFormat, ...)
	{
		wchar_t buffer[sg_uiBufferCount];

		va_list args = nullptr;
		va_start(args, cpFormat);
		size_t cch = Platform::Sprintf_V(buffer, sg_uiBufferCount, cpFormat, args);
		va_end(args);

		return (cch <= 0) ? (false) : (Put(buffer, cch));
	}
}
