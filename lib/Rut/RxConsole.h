#pragma once
#include <string>
#include <corecrt_wstdio.h>


namespace Rut::RxConsole
{
	FILE* Alloc(const wchar_t* lpTitle, bool isEdit = false, bool isTop = true);
}

namespace Rut::RxConsole
{
	bool Put(const char* cpStr);
	bool Put(const char* cpStr, size_t nChar);
	bool Put(std::string_view msStr);
	bool Put(const wchar_t* wpStr);
	bool Put(const wchar_t* wpStr, size_t nChar);
	bool Put(std::wstring_view wsStr);
	bool PutFormat(const char* cpFormat, ...);
	bool PutFormat(const wchar_t* cpFormat, ...);
	bool PutMBCS(const char* cpStr, size_t nCodePage);
	bool PutMBCS(std::string_view msStr, size_t nCodePage);
}

