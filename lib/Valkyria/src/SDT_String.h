#pragma once
#include <string>


namespace Valkyria::SDT
{
	class String
	{
	public:
		static std::string Decode(uint8_t* pStr);
		static void Encode(uint8_t* pStr);
		static std::string MakeText(std::wstring_view wsText, size_t nCodePage);
	};
}