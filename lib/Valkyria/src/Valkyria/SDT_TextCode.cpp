#include "SDT_TextCode.h"


namespace Valkyria::SDT
{
	TextCode::TextCode()
	{

	}

	TextCode::TextCode(const uint8_t* const pCodeSeg, size_t uiCodeOffset)
	{
		this->Load(pCodeSeg, uiCodeOffset);
	}

	void TextCode::Load(const uint8_t* const pCodeSeg, size_t uiCodeOffset)
	{
		const uint8_t* cur_ptr = pCodeSeg + uiCodeOffset;

		const uint16_t op_code = *((uint16_t*)cur_ptr);

		switch (op_code)
		{
		case 0x0E00: m_vaCode = SDT::Code::MsgName{}; break;
		case 0x0E01: m_vaCode = SDT::Code::MsgText{}; break;
		case 0x0E04: m_vaCode = SDT::Code::MsgNewLine{}; break;
		case 0x0E1C: m_vaCode = SDT::Code::SelectText{}; break;
		case 0x0B17: m_vaCode = SDT::Code::SetStr{}; break;
		}

		std::visit([&cur_ptr](auto&& obj) {obj.Load(cur_ptr); cur_ptr += obj.GetSize(); }, m_vaCode);

		m_uiBegOffset = uiCodeOffset;
		m_uiEndOffset = (size_t)(cur_ptr - pCodeSeg);
	}

	void TextCode::Load(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		std::visit([&rfJson, nCodePage](auto&& obj) {obj.Load(rfJson, nCodePage); }, m_vaCode);
	}

	void TextCode::Make(Rut::RxMem::Auto& rfMem) const
	{
		std::visit([&rfMem](auto&& obj) {obj.Make(rfMem); }, m_vaCode);
	}


	Rut::RxJson::JValue TextCode::Make(size_t nCodePage) const
	{
		Rut::RxJson::JValue jvalue;
		std::visit([&jvalue, nCodePage](auto&& obj) {jvalue = obj.Make(nCodePage); }, m_vaCode);
		return jvalue;
	}

	constexpr size_t TextCode::GetSize() const
	{
		size_t size = 0;
		std::visit([&size](auto&& obj) {size = obj.GetSize(); }, m_vaCode);
		return size;
	}

	constexpr size_t TextCode::GetBegOffset() const noexcept
	{
		return m_uiBegOffset;
	}

	constexpr size_t TextCode::GetEndOffset() const noexcept
	{
		return m_uiEndOffset;
	}


	Text_Code_Block::Text_Code_Block()
	{

	}

	Text_Code_Block::Text_Code_Block(std::vector<TextCode>& vcCodeList)
	{
		this->Load(vcCodeList);
	}

	void Text_Code_Block::Load(std::vector<TextCode>& vcCodeList)
	{
		m_uiBegOffset = vcCodeList.front().GetBegOffset();
		m_uiEndOffset = vcCodeList.back().GetEndOffset();

		size_t block_size = 0;
		for (auto& msg_code : vcCodeList) { block_size += msg_code.GetSize(); }
		m_amMem.SetSize(block_size);

		size_t wrire_bytes = 0;
		Rut::RxMem::Auto buffer;
		for (auto& msg_code : vcCodeList)
		{
			msg_code.Make(buffer);
			memcpy(m_amMem.GetPtr() + wrire_bytes, buffer.GetPtr(), buffer.GetSize());
			wrire_bytes += buffer.GetSize();
		}
	}

	uint8_t* Text_Code_Block::GetPtr() const noexcept
	{
		return m_amMem.GetPtr();
	}

	const size_t Text_Code_Block::GetSize() const noexcept
	{
		return m_amMem.GetSize();
	}

	const size_t Text_Code_Block::GetBegOffset() const noexcept
	{
		return m_uiBegOffset;
	}

	const size_t Text_Code_Block::GetEndOffset() const noexcept
	{
		return m_uiEndOffset;
	}
}