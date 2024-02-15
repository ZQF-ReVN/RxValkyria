#pragma once
#include <string>


namespace Valkyria::SDT
{
	class String
	{
	public:
		static void Encode(uint8_t* cpStr);
		static std::string Decode(const uint8_t* const pData);

		static std::string MakeText(std::wstring_view wsText, size_t nCodePage);
		static std::wstring LoadText(std::string_view msText, size_t nCodePage);

		static std::wstring NumToStr(const wchar_t* wpFormat, size_t nValue);
		static size_t StrToNum(const wchar_t* wpFormat, std::wstring_view wsText);
	};
}