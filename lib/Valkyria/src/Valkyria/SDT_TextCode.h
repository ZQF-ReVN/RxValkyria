#pragma once
#include <variant>

#include "SDT_Code.h"


namespace Valkyria::SDT
{
	using Text_Code_Variant = std::variant<Code::MsgName, Code::MsgText, Code::MsgNewLine, Code::SelectText, Code::SetStr>;

	class TextCode
	{
	private:
		size_t m_uiBegOffset = 0;
		size_t m_uiEndOffset = 0;
		Text_Code_Variant m_vaCode;

	public:
		TextCode();
		TextCode(uint8_t* const pCodeSeg, size_t uiCodeOffset);
		void Load(uint8_t* pCodeSeg, size_t uiCodeOffset);
		void Load(Rut::RxJson::JValue& rfJson, size_t nCodePage);
		void Make(Rut::RxMem::Auto& rfMem) const;
		Rut::RxJson::JValue Make(size_t nCodePage) const;

	public:
		constexpr size_t GetSize() const;
		constexpr size_t GetBegOffset() const noexcept;
		constexpr size_t GetEndOffset() const noexcept;
	};

	class Text_Code_Block
	{
	private:
		size_t m_uiBegOffset = 0;
		size_t m_uiEndOffset = 0;
		Rut::RxMem::Auto m_amMem;

	public:
		Text_Code_Block();
		Text_Code_Block(std::vector<TextCode>& vcCodeList);

		void Load(std::vector<TextCode>& vcCodeList);

	public:
		uint8_t* GetPtr() const noexcept;
		const size_t GetSize() const noexcept;
		const size_t GetBegOffset() const noexcept;
		const size_t GetEndOffset() const noexcept;
	};
}