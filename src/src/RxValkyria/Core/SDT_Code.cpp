#include <RxValkyria/Core/SDT_Code.h>
#include <RxValkyria/Core/SDT_String.h>

#include <cassert>


namespace ZQF::RxValkyria::SDT::Code
{
	MsgName::MsgName()
	{

	}

	MsgName::MsgName(const std::uint8_t* const pData)
	{
		this->Load(pData);
	}

	auto MsgName::Load(const std::uint8_t* const pData) -> void
	{
		m_usOP = *((uint16_t*)pData + 0); assert(m_usOP == 0x0E00);
		m_msText = String::Decode(pData + sizeof(m_usOP));
	}

	auto MsgName::Load(const ZxJson::JValue& rfJson, std::size_t nCodePage) -> void
	{
		assert(rfJson.At("Name").GetStrView() == "MsgName");
		m_usOP = (uint16_t)String::StrToNum("0x%04x", rfJson.At("OP").GetStrView());
		m_msText = String::MakeText(rfJson.At("Text").GetStrView(), nCodePage);
	}

	auto MsgName::Make(ZxMem& rfMem) const -> void
	{
		rfMem << static_cast<std::uint16_t>(m_usOP);

		auto text_ptr = rfMem.PtrCur<uint8_t*>();
		rfMem << std::span{ m_msText.data(), m_msText.size() + 1 };
		String::Encode(text_ptr);
	}

	auto MsgName::Make(std::size_t nCodePage) const -> ZxJson::JValue
	{
		return ZxJson::JObject_t
		{
			{ "Name", "MsgName" },
			{ "OP", String::NumToStr("0x%04x", m_usOP) },
			{ "Text",  String::LoadText(m_msText, nCodePage) }
		};
	}

	auto MsgName::GetOP() const -> std::uint16_t
	{
		return m_usOP;
	}

	auto MsgName::GetText(std::size_t nCodePage) const -> std::string
	{
		return String::LoadText(m_msText, nCodePage);
	}

	auto MsgName::SetText(std::string_view wsText, std::size_t nCodePage) -> void
	{
		m_msText = String::MakeText(wsText, nCodePage);
	}

	auto MsgName::SizeBytes() const -> std::size_t
	{
		assert(m_usOP != 0);
		return sizeof(m_usOP) + m_msText.size() + 1;
	}


	MsgText::MsgText()
	{

	}

	MsgText::MsgText(const std::uint8_t* const pData)
	{
		this->Load(pData);
	}

	auto MsgText::Load(const std::uint8_t* const pData) -> void
	{
		m_usOP = *((uint16_t*)pData + 0); assert(m_usOP == 0x0E01);
		m_usUnknow = *((uint16_t*)pData + 1); assert(m_usUnknow == 0x1111);
		m_uiLineNumber = *((uint32_t*)pData + 1);
		m_msText = String::Decode(pData + 8);
	}


	auto MsgText::Load(const ZxJson::JValue& rfJson, std::size_t nCodePage) -> void
	{
		assert(rfJson.At("Name").GetStrView() == "MsgText");
		m_usOP = (uint16_t)String::StrToNum("0x%04x", rfJson.At("OP").GetStrView());
		m_usUnknow = (uint16_t)String::StrToNum("0x%04x", rfJson.At("Unknow").GetStrView());
		m_uiLineNumber = (uint32_t)String::StrToNum("0x%08x", rfJson.At("LineNumber").GetStrView());
		m_msText = String::MakeText(rfJson.At("Text").GetStrView(), nCodePage);
	}

	auto MsgText::Make(ZxMem& rfMem) const -> void
	{
		rfMem << static_cast<std::uint16_t>(m_usOP);
		rfMem << static_cast<std::uint16_t>(m_usUnknow);
		rfMem << static_cast<std::uint32_t>(m_uiLineNumber);

		auto text_ptr = rfMem.PtrCur<uint8_t*>();
		rfMem << std::span{ m_msText.data(), m_msText.size() + 1 };
		String::Encode(text_ptr);
	}

	auto MsgText::Make(std::size_t nCodePage) const -> ZxJson::JValue
	{
		return ZxJson::JObject_t
		{
			{ "Name", "MsgText" },
			{ "OP", String::NumToStr("0x%04x", m_usOP) },
			{ "Unknow", String::NumToStr("0x%04x", m_usUnknow) },
			{ "LineNumber", String::NumToStr("0x%08x", m_uiLineNumber) },
			{ "Text", String::LoadText(m_msText, nCodePage) }
		};
	}

	auto MsgText::GetOP() const -> std::uint16_t
	{
		return m_usOP;
	}

	auto MsgText::GetText(std::size_t nCodePage) const -> std::string
	{
		return String::LoadText(m_msText, nCodePage);
	}

	auto MsgText::SetText(std::string_view wsText, std::size_t nCodePage) -> void
	{
		m_msText = String::MakeText(wsText, nCodePage);
	}

	auto MsgText::SizeBytes() const -> std::size_t
	{
		assert(m_usOP != 0);
		return sizeof(m_usOP) + sizeof(m_usUnknow) + sizeof(m_uiLineNumber) + (m_msText.size() + 1);
	}


	MsgNewLine::MsgNewLine()
	{

	}

	MsgNewLine::MsgNewLine(const std::uint8_t* const pData)
	{
		this->Load(pData);
	}

	auto MsgNewLine::Load(const std::uint8_t* const pData) -> void
	{
		m_usOP = *((uint16_t*)(pData + 0)); assert(m_usOP == 0x0E04);
		m_usUnknow = *((uint16_t*)(pData + 2)); assert(m_usUnknow == 0x1111);
		m_uiLineNumber = *((uint32_t*)(pData + 4));
	}

	auto MsgNewLine::Load(const ZxJson::JValue& rfJson, std::size_t /*nCodePage*/) -> void
	{
		assert(rfJson.At("Name").GetStrView() == "MsgNewLine");
		m_usOP = (uint16_t)String::StrToNum("0x%04x", rfJson.At("OP").GetStrView());
		m_usUnknow = (uint16_t)String::StrToNum("0x%04x", rfJson.At("Unknow").GetStrView());
		m_uiLineNumber = (uint32_t)String::StrToNum("0x%08x", rfJson.At("LineNumber").GetStrView());
	}

	auto MsgNewLine::Make(ZxMem& rfMem) const -> void
	{
		rfMem << static_cast<std::uint16_t>(m_usOP);
		rfMem << static_cast<std::uint16_t>(m_usUnknow);
		rfMem << static_cast<std::uint32_t>(m_uiLineNumber);
	}

	auto MsgNewLine::Make(std::size_t /*nCodePage*/) const -> ZxJson::JValue
	{
		return ZxJson::JObject_t
		{
			{ "Name", "MsgNewLine" },
			{ "OP", String::NumToStr("0x%04x", m_usOP) },
			{ "Unknow", String::NumToStr("0x%04x", m_usUnknow) },
			{ "LineNumber", String::NumToStr("0x%08x", m_uiLineNumber) }
		};
	}

	auto MsgNewLine::GetOP() const -> std::uint16_t
	{
		return m_usOP;
	}

	auto MsgNewLine::SizeBytes() const -> std::size_t
	{
		assert(m_usOP != 0);
		return sizeof(m_usOP) + sizeof(m_usUnknow) + sizeof(m_uiLineNumber);
	}


	SelectText::SelectText()
	{

	}

	SelectText::SelectText(const std::uint8_t* const pData)
	{
		this->Load(pData);
	}

	auto SelectText::Load(const std::uint8_t* const pData) -> void
	{
		m_usOP = *((uint16_t*)(pData + 0)); assert(m_usOP == 0x0E1C);
		m_usUnknow = *((uint32_t*)(pData + 2)); assert(m_usUnknow == 0x00000000);

		const uint8_t* text_array_ptr = pData + 6;
		while (true)
		{
			uint8_t type = text_array_ptr[0];
			if (type == 0x8)
			{
				std::string text = String::Decode(text_array_ptr + 1);
				text_array_ptr += sizeof(type) + text.size() + 1;
				m_vcText.push_back(std::move(text));
			}
			else if (type == 0xFF)
			{
				break;
			}
			else
			{
				throw std::runtime_error("SDT_Code_SelectText::Parse Error!");
			}
		}
	}

	auto SelectText::Load(const ZxJson::JValue& rfJson, std::size_t nCodePage) -> void
	{
		assert(rfJson.At("Name").GetStrView() == "SelectText");
		m_usOP = (uint16_t)String::StrToNum("0x%04x", rfJson.At("OP").GetStrView());
		m_usUnknow = (uint32_t)String::StrToNum("0x%08x", rfJson.At("Unknow").GetStrView());

		m_vcText.clear();
		for (auto& text : rfJson.At("Text").GetArray())
		{
			m_vcText.push_back(String::MakeText(text.GetStrView(), nCodePage));
		}
	}

	auto SelectText::Make(ZxMem& rfMem) const -> void
	{
		rfMem
			<< static_cast<std::uint16_t>(m_usOP)
			<< static_cast<std::uint32_t>(m_usUnknow);

		for (auto& text : m_vcText)
		{
			rfMem << static_cast<std::uint8_t>(0x8); // Write Type

			auto text_ptr = rfMem.PtrCur<uint8_t*>();
			rfMem << std::span{ text.data(), text.size() + 1 };
			String::Encode(text_ptr);
		}

		rfMem << static_cast<std::uint8_t>(0xFF); // End Array
	}

	auto SelectText::Make(std::size_t nCodePage) const -> ZxJson::JValue
	{
		ZxJson::JArray_t text_vec;
		for (auto& text : m_vcText)
		{
			text_vec.emplace_back(String::LoadText(text, nCodePage));
		}

		return ZxJson::JObject_t
		{
			{ "Name", "SelectText"},
			{ "OP", String::NumToStr("0x%04x", m_usOP) },
			{ "Unknow", String::NumToStr("0x%08x", m_usUnknow) },
			{ "Text", std::move(text_vec) }
		};
	}

	auto SelectText::GetOP() const -> std::uint16_t
	{
		return m_usOP;
	}

	auto SelectText::SizeBytes() const -> std::size_t
	{
		assert(m_usOP != 0);

		size_t size = sizeof(m_usOP) + sizeof(m_usUnknow);
		for (auto& text : m_vcText) { size += 1 + text.size() + 1; }
		size += 0x1; // end array flag
		return size;
	}


	SetStr::SetStr()
	{

	}

	SetStr::SetStr(const std::uint8_t* const pData)
	{
		this->Load(pData);
	}

	auto SetStr::Load(const std::uint8_t* const pData) -> void
	{
		const uint8_t* cur_ptr = pData;

		m_usOP = *((uint16_t*)cur_ptr); assert(m_usOP == 0x0B17);
		cur_ptr += 2;
		m_ucStrType = *((uint8_t*)cur_ptr); assert(m_ucStrType == 0x9);
		cur_ptr += 1;
		m_uiSPIndex = *((uint32_t*)cur_ptr); assert(m_uiSPIndex == 0x00000000);
		cur_ptr += 4;
		m_ucStrDataType = *((uint8_t*)cur_ptr); assert(m_ucStrDataType == 0x8);
		cur_ptr += 1;
		m_msText = String::Decode(cur_ptr);
	}

	auto SetStr::Load(const ZxJson::JValue& rfJson, std::size_t nCodePage) -> void
	{
		assert(rfJson.At("Name").GetStrView() == "SetStr");
		m_usOP = (uint16_t)String::StrToNum("0x%04x", rfJson.At("OP").GetStrView());
		m_ucStrType = (uint8_t)String::StrToNum("0x%02x", rfJson.At("StrType").GetStrView());
		m_uiSPIndex = (uint32_t)String::StrToNum("0x%08x", rfJson.At("SPIndex").GetStrView());
		m_ucStrDataType = (uint8_t)String::StrToNum("0x%02x", rfJson.At("StrDataType").GetStrView());
		m_msText = String::MakeText(rfJson.At("Text").GetStrView(), nCodePage);
	}

	auto SetStr::Make(ZxMem& rfMem) const -> void
	{
		rfMem
			<< static_cast<std::uint16_t>(m_usOP)
			<< static_cast<std::uint8_t>(m_ucStrType)
			<< static_cast<std::uint32_t>(m_uiSPIndex)
			<< static_cast<std::uint8_t>(m_ucStrDataType);

		auto text_ptr = rfMem.PtrCur<uint8_t*>();
		rfMem << std::span{ m_msText.data(), m_msText.size() + 1 };
		String::Encode(text_ptr);
	}

	auto SetStr::Make(std::size_t nCodePage) const -> ZxJson::JValue
	{
		return ZxJson::JObject_t
		{
			{ "Name", "SetStr" },
			{ "OP", String::NumToStr("0x%04x", m_usOP) },
			{ "StrType", String::NumToStr("0x%02x", m_ucStrType) },
			{ "SPIndex", String::NumToStr("0x%08x", m_uiSPIndex) },
			{ "StrDataType", String::NumToStr("0x%02x", m_ucStrDataType) },
			{ "Text", String::LoadText(m_msText, nCodePage) }
		};
	}

	auto SetStr::SizeBytes() const -> std::size_t
	{
		assert(m_usOP != 0);
		return sizeof(m_usOP) + sizeof(m_ucStrType) + sizeof(m_uiSPIndex) + sizeof(m_ucStrDataType) + m_msText.size() + 1;
	}

	auto SetStr::GetOP() const -> std::uint16_t
	{
		return m_usOP;
	}

	auto SetStr::GetText(std::size_t nCodePage) const -> std::string
	{
		return String::LoadText(m_msText, nCodePage);
	}

	auto SetStr::SetText(std::string_view wsText, std::size_t nCodePage) -> void
	{
		m_msText = String::MakeText(wsText, nCodePage);
	}
}