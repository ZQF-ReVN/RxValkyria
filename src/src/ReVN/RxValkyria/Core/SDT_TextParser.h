#pragma once
#include <ranges>
#include <algorithm>
#include <ReVN/RxValkyria/Core/SDT_View.h>
#include <ReVN/RxValkyria/Core/SDT_TextCode.h>
#include <ReVN/RxValkyria/Core/Valkyria_Struct.h>


namespace ZQF::ReVN::RxValkyria::SDT
{
	class TextParser
	{
	private:
		ZxMem m_mSdt;
		std::vector<SDT::TextCode> m_vcMsg;

	public:
		TextParser();
		TextParser(const std::string_view msSdtPath);

	public:
		auto Scan() -> void;

	public:
		auto LoadViaFile(const std::string_view msSdtPath) -> void;
		auto LoadViaJson(const ZxJson::JArray_t& rfMsgJson, const std::size_t nCodePage) -> void;
		auto MakeBin() const -> ZxMem;
		auto MakeJson(const std::size_t nCodePage) const -> ZxJson::JArray_t;

	public:
		auto GetMsgCnt() const noexcept -> std::size_t;
		auto GetSdtFile() const noexcept -> const ZxMem&;
	};

	class TextParserJsonManager : private TextParser
	{
	public:
		TextParserJsonManager(const std::string_view msSdtPath);

	public:
		auto Parse() -> bool;
		auto Export(const std::size_t nCodePage) const -> ZxJson::JArray_t;
		auto Export(const std::string_view msJsonPath, const std::size_t nCodePage) const -> void;
		auto Import(const std::string_view msJsonPath, const std::size_t nCodePage) -> void;
		auto Import(const std::string_view msSdtPath, const std::string_view msJsonPath, const std::size_t nCodePage) -> void;
	};
}
