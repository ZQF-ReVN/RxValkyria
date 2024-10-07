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
		auto BinaryStore() const -> ZxMem;
		auto BinaryLoad(const std::string_view msSdtPath) -> void;
		auto MetaStore(const std::size_t nCodePage) const -> ZxJson::JValue;
		auto MetaLoad(const ZxJson::JValue& rfMsgJson, const std::size_t nCodePage) -> void;
	
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
