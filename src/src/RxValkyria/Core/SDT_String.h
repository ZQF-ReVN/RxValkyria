#pragma once
#include <cstdint>
#include <string>
#include <string_view>


namespace ZQF::RxValkyria::SDT
{
	class String
	{
	public:
		static auto Encode(std::uint8_t* cpStr) -> void;
		static auto Decode(const std::uint8_t* const pData) -> std::string;

		static auto MakeText(const std::string_view wsText, const std::size_t nCodePage) -> std::string;
		static auto LoadText(const std::string_view msText, const std::size_t nCodePage) -> std::string;

		static auto NumToStr(const char* cpFormat, const std::size_t nValue) -> std::string;
		static auto StrToNum(const char* cpFormat, const std::string_view wsText) -> std::size_t;
	};
}