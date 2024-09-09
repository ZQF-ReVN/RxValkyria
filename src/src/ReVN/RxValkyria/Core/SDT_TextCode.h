#pragma once
#include <variant>
#include <Zut/ZxMem.h>
#include <Zut/ZxJson.h>
#include <ReVN/RxValkyria/Core/SDT_Code.h>


namespace ZQF::ReVN::RxValkyria::SDT
{
	class TextCode
	{
	private:
		using Text_Code_Variant = std::variant<Code::MsgName, Code::MsgText, Code::MsgNewLine, Code::SelectText, Code::SetStr>;

	private:
		std::size_t m_uiBegOffset{};
		std::size_t m_uiEndOffset{};
		Text_Code_Variant m_vaCode;

	public:
		TextCode();
		TextCode(const uint8_t* const pCodeSeg, const std::size_t uiCodeOffset);

	public:
		auto Load(const uint8_t* const pCodeSeg, const std::size_t uiCodeOffset) -> void;
		auto Load(const ZxJson::JValue& rfJson, const std::size_t nCodePage) -> void;
		auto Make(ZxMem& rfMem) const -> void;
		auto Make(const std::size_t nCodePage) const -> ZxJson::JValue;

	public:
		auto GetBegOffset() const noexcept -> std::size_t;
		auto GetEndOffset() const noexcept -> std::size_t;

	public:
		auto SizeBytes() const -> std::size_t;
	};

	class TextCodeBlock
	{
	private:
		std::size_t m_uiBegOffset{};
		std::size_t m_uiEndOffset{};
		ZxMem m_amMem;

	public:
		TextCodeBlock();
		TextCodeBlock(std::vector<TextCode>& vcCodeList);

	public:
		auto Load(std::vector<TextCode>& vcCodeList) -> void;

	public:
		auto GetMem() const noexcept -> const ZxMem&;
		auto GetBegOffset() const noexcept -> std::size_t;
		auto GetEndOffset() const noexcept -> std::size_t;

	public:
		auto SizeBytes() const noexcept -> std::size_t;
	};
}