#include "SDT_TextCode.h"


namespace ZQF::ReVN::RxValkyria::SDT
{
	TextCode::TextCode()
	{

	}

	TextCode::TextCode(const uint8_t* const pCodeSeg, const std::size_t uiCodeOffset)
	{
		this->Load(pCodeSeg, uiCodeOffset);
	}

	auto TextCode::Load(const uint8_t* const pCodeSeg, const std::size_t uiCodeOffset) -> void
	{
		auto cur_ptr = pCodeSeg + uiCodeOffset;

		const auto op_code = *reinterpret_cast<const std::uint16_t*>(cur_ptr);
		switch (op_code)
		{
		case 0x0E00: m_vaCode = SDT::Code::MsgName{}; break;
		case 0x0E01: m_vaCode = SDT::Code::MsgText{}; break;
		case 0x0E04: m_vaCode = SDT::Code::MsgNewLine{}; break;
		case 0x0E1C: m_vaCode = SDT::Code::SelectText{}; break;
		case 0x0B17: m_vaCode = SDT::Code::SetStr{}; break;
		}

		std::visit([&cur_ptr](auto&& obj) { obj.Load(cur_ptr); cur_ptr += obj.SizeBytes(); }, m_vaCode);

		m_uiBegOffset = uiCodeOffset;
		m_uiEndOffset = static_cast<std::size_t>(cur_ptr - pCodeSeg);
	}

	auto TextCode::Load(const ZxJson::JValue& rfJson, const std::size_t nCodePage) -> void
	{
		std::visit([&rfJson, nCodePage](auto&& obj) { obj.Load(rfJson, nCodePage); }, m_vaCode);
	}

	auto TextCode::Make(ZxMem& rfMem) const -> void
	{
		std::visit([&rfMem](auto&& obj) { obj.Make(rfMem); }, m_vaCode);
	}


	auto TextCode::Make(const std::size_t nCodePage) const -> ZxJson::JValue
	{
		ZxJson::JValue jvalue;
		std::visit([&jvalue, nCodePage](auto&& obj) { jvalue = obj.Make(nCodePage); }, m_vaCode);
		return jvalue;
	}

	auto TextCode::GetBegOffset() const noexcept -> std::size_t
	{
		return m_uiBegOffset;
	}

	auto TextCode::GetEndOffset() const noexcept -> std::size_t
	{
		return m_uiEndOffset;
	}

	auto TextCode::SizeBytes() const -> std::size_t
	{
		std::size_t bytes{};
		std::visit([&bytes](auto&& obj) { bytes = obj.SizeBytes(); }, m_vaCode);
		return bytes;
	}


	TextCodeBlock::TextCodeBlock()
	{

	}

	TextCodeBlock::TextCodeBlock(std::vector<TextCode>& vcCodeList)
	{
		this->Load(vcCodeList);
	}

	auto TextCodeBlock::Load(std::vector<TextCode>& vcCodeList) -> void
	{
		m_uiBegOffset = vcCodeList.front().GetBegOffset();
		m_uiEndOffset = vcCodeList.back().GetEndOffset();

		std::size_t block_bytes{};
		for (auto& msg_code : vcCodeList) { block_bytes += msg_code.SizeBytes(); }
		m_amMem.Resize(block_bytes);

		for (auto& msg_code : vcCodeList)
		{
			msg_code.Make(m_amMem);
		}
	}

	auto TextCodeBlock::GetMem() const noexcept -> const ZxMem&
	{
		return m_amMem;
	}

	auto TextCodeBlock::GetBegOffset() const noexcept -> std::size_t
	{
		return m_uiBegOffset;
	}

	auto TextCodeBlock::GetEndOffset() const noexcept -> std::size_t
	{
		return m_uiEndOffset;
	}

	auto TextCodeBlock::SizeBytes() const noexcept -> std::size_t
	{
		return m_amMem.SizeBytes();
	}
}