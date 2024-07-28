#include "SDT_Code.h"
#include "SDT_String.h"

#include <cassert>


namespace Valkyria::SDT::Code
{
	MsgName::MsgName()
	{

	}

	MsgName::MsgName(const uint8_t* const pData)
	{
		this->Load(pData);
	}

	void MsgName::Load(const uint8_t* const pData)
	{
		m_usOP = *((uint16_t*)pData + 0); assert(m_usOP == 0x0E00);
		m_msText = String::Decode(pData + sizeof(m_usOP));
	}

	void MsgName::Load(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		assert(rfJson[L"Name"].ToStrView() == L"MsgName");
		m_usOP = (uint16_t)String::StrToNum(L"0x%04x", rfJson[L"OP"]);
		m_msText = String::MakeText(rfJson[L"Text"], nCodePage);
	}

	void MsgName::Make(Rut::RxMem::Auto& rfMem) const
	{
		rfMem.SetSize(this->GetSize());
		uint8_t* cur_ptr = rfMem.GetPtr();

		(*(uint16_t*)cur_ptr) = m_usOP;
		cur_ptr += sizeof(m_usOP);
		memcpy(cur_ptr, m_msText.data(), m_msText.size() + 1);
		String::Encode(cur_ptr);
	}

	Rut::RxJson::JValue MsgName::Make(size_t nCodePage) const
	{
		Rut::RxJson::JValue json;
		json[L"Name"] = L"MsgName";
		json[L"OP"] = String::NumToStr(L"0x%04x", m_usOP);
		json[L"Text"] = String::LoadText(m_msText, nCodePage);
		return json;
	}

	constexpr uint16_t MsgName::GetOP() const
	{
		return m_usOP;
	}

	constexpr size_t MsgName::GetSize() const
	{
		assert(m_usOP != 0);
		return sizeof(m_usOP) + m_msText.size() + 1;
	}

	std::wstring MsgName::GetText(size_t nCodePage) const
	{
		return String::LoadText(m_msText, nCodePage);
	}

	void MsgName::SetText(std::wstring_view wsText, size_t nCodePage)
	{
		m_msText = String::MakeText(wsText, nCodePage);
	}


	MsgText::MsgText()
	{

	}

	MsgText::MsgText(const uint8_t* const pData)
	{
		this->Load(pData);
	}

	void MsgText::Load(const uint8_t* const pData)
	{
		m_usOP = *((uint16_t*)pData + 0); assert(m_usOP == 0x0E01);
		m_usUnknow = *((uint16_t*)pData + 1); assert(m_usUnknow == 0x1111);
		m_uiLineNumber = *((uint32_t*)pData + 1);
		m_msText = String::Decode(pData + 8);
	}


	void MsgText::Load(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		assert(rfJson[L"Name"].ToStrView() == L"MsgText");
		m_usOP = (uint16_t)String::StrToNum(L"0x%04x", rfJson[L"OP"]);
		m_usUnknow = (uint16_t)String::StrToNum(L"0x%04x", rfJson[L"Unknow"]);
		m_uiLineNumber = (uint32_t)String::StrToNum(L"0x%08x", rfJson[L"LineNumber"]);
		m_msText = String::MakeText(rfJson[L"Text"], nCodePage);
	}

	void MsgText::Make(Rut::RxMem::Auto& rfMem) const
	{
		rfMem.SetSize(this->GetSize());
		uint8_t* cur_ptr = rfMem.GetPtr();

		(*(uint16_t*)cur_ptr) = m_usOP;
		cur_ptr += sizeof(m_usOP);
		(*(uint16_t*)cur_ptr) = m_usUnknow;
		cur_ptr += sizeof(m_usUnknow);
		(*(uint32_t*)cur_ptr) = m_uiLineNumber;
		cur_ptr += sizeof(m_uiLineNumber);
		memcpy(cur_ptr, m_msText.data(), m_msText.size() + 1);
		String::Encode(cur_ptr);
	}

	Rut::RxJson::JValue MsgText::Make(size_t nCodePage) const
	{
		Rut::RxJson::JValue json;
		json[L"Name"] = L"MsgText";
		json[L"OP"] = String::NumToStr(L"0x%04x", m_usOP);
		json[L"Unknow"] = String::NumToStr(L"0x%04x", m_usUnknow);
		json[L"LineNumber"] = String::NumToStr(L"0x%08x", m_uiLineNumber);
		json[L"Text"] = String::LoadText(m_msText, nCodePage);
		return json;
	}

	constexpr uint16_t MsgText::GetOP() const
	{
		return m_usOP;
	}

	constexpr size_t MsgText::GetSize() const
	{
		assert(m_usOP != 0);
		return sizeof(m_usOP) + sizeof(m_usUnknow) + sizeof(m_uiLineNumber) + (m_msText.size() + 1);
	}

	std::wstring MsgText::GetText(size_t nCodePage) const
	{
		return String::LoadText(m_msText, nCodePage);
	}

	void MsgText::SetText(std::wstring_view wsText, size_t nCodePage)
	{
		m_msText = String::MakeText(wsText, nCodePage);
	}


	MsgNewLine::MsgNewLine()
	{

	}

	MsgNewLine::MsgNewLine(const uint8_t* const pData)
	{
		this->Load(pData);
	}

	void MsgNewLine::Load(const uint8_t* const pData)
	{
		m_usOP = *((uint16_t*)(pData + 0)); assert(m_usOP == 0x0E04);
		m_usUnknow = *((uint16_t*)(pData + 2)); assert(m_usUnknow == 0x1111);
		m_uiLineNumber = *((uint32_t*)(pData + 4));
	}

	void MsgNewLine::Load(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		assert(rfJson[L"Name"].ToStrView() == L"MsgNewLine");
		m_usOP = (uint16_t)String::StrToNum(L"0x%04x", rfJson[L"OP"]);
		m_usUnknow = (uint16_t)String::StrToNum(L"0x%04x", rfJson[L"Unknow"]);
		m_uiLineNumber = (uint32_t)String::StrToNum(L"0x%08x", rfJson[L"LineNumber"]);
	}

	void MsgNewLine::Make(Rut::RxMem::Auto& rfMem) const
	{
		rfMem.SetSize(this->GetSize());
		uint8_t* cur_ptr = rfMem.GetPtr();

		(*(uint16_t*)cur_ptr) = m_usOP;
		cur_ptr += sizeof(m_usOP);
		(*(uint16_t*)cur_ptr) = m_usUnknow;
		cur_ptr += sizeof(m_usUnknow);
		(*(uint32_t*)cur_ptr) = m_uiLineNumber;
		cur_ptr += sizeof(m_uiLineNumber);
	}

	Rut::RxJson::JValue MsgNewLine::Make(size_t nCodePage) const
	{
		Rut::RxJson::JValue json;
		json[L"Name"] = L"MsgNewLine";
		json[L"OP"] = String::NumToStr(L"0x%04x", m_usOP);
		json[L"Unknow"] = String::NumToStr(L"0x%04x", m_usUnknow);
		json[L"LineNumber"] = String::NumToStr(L"0x%08x", m_uiLineNumber);
		return json;
	}

	constexpr uint16_t MsgNewLine::GetOP() const
	{
		return m_usOP;
	}

	constexpr size_t MsgNewLine::GetSize() const
	{
		assert(m_usOP != 0);
		return sizeof(m_usOP) + sizeof(m_usUnknow) + sizeof(m_uiLineNumber);
	}


	SelectText::SelectText()
	{

	}

	SelectText::SelectText(const uint8_t* const pData)
	{
		this->Load(pData);
	}

	void SelectText::Load(const uint8_t* const pData)
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

	void SelectText::Load(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		assert(rfJson[L"Name"].ToStrView() == L"SelectText");
		m_usOP = (uint16_t)String::StrToNum(L"0x%04x", rfJson[L"OP"]);
		m_usUnknow = (uint32_t)String::StrToNum(L"0x%08x", rfJson[L"Unknow"]);

		m_vcText.clear();
		Rut::RxJson::JArray& text_list = rfJson[L"Text"].ToAry();
		for (auto& text : text_list)
		{
			m_vcText.push_back(String::MakeText(text, nCodePage));
		}
	}

	void SelectText::Make(Rut::RxMem::Auto& rfMem) const
	{
		rfMem.SetSize(this->GetSize());
		uint8_t* cur_ptr = rfMem.GetPtr();

		(*(uint16_t*)cur_ptr) = m_usOP;
		cur_ptr += sizeof(m_usOP);
		(*(uint32_t*)cur_ptr) = m_usUnknow;
		cur_ptr += sizeof(m_usUnknow);
		for (auto& text : m_vcText)
		{
			*cur_ptr = 0x8; // Write Type
			memcpy(cur_ptr + 1, text.data(), text.size() + 1);
			String::Encode(cur_ptr + 1);
			cur_ptr += 1 + text.size() + 1;
		}
		*cur_ptr = 0xFF; // End Array
	}

	Rut::RxJson::JValue SelectText::Make(size_t nCodePage) const
	{
		Rut::RxJson::JValue json;
		json[L"Name"] = L"SelectText";
		json[L"OP"] = String::NumToStr(L"0x%04x", m_usOP);
		json[L"Unknow"] = String::NumToStr(L"0x%08x", m_usUnknow);

		Rut::RxJson::JArray& j_text_array = json[L"Text"].ToAry();
		for (auto& text : m_vcText) 
		{ 
			j_text_array.emplace_back(String::LoadText(text, nCodePage));
		}

		return json;
	}

	constexpr uint16_t SelectText::GetOP() const
	{
		return m_usOP;
	}

	constexpr size_t SelectText::GetSize() const
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

	SetStr::SetStr(const uint8_t* const pData)
	{
		this->Load(pData);
	}

	void SetStr::Load(const uint8_t* const pData)
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

	void SetStr::Load(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		assert(rfJson[L"Name"].ToStrView() == L"SetStr");
		m_usOP = (uint16_t)String::StrToNum(L"0x%04x", rfJson[L"OP"]);
		m_ucStrType = (uint8_t)String::StrToNum(L"0x%02x", rfJson[L"StrType"]);
		m_uiSPIndex = (uint32_t)String::StrToNum(L"0x%08x", rfJson[L"SPIndex"]);
		m_ucStrDataType = (uint8_t)String::StrToNum(L"0x%02x", rfJson[L"StrDataType"]);
		m_msText = String::MakeText(rfJson[L"Text"], nCodePage);
	}

	void SetStr::Make(Rut::RxMem::Auto& rfMem) const
	{
		rfMem.SetSize(this->GetSize());

		uint8_t* cur_ptr = rfMem.GetPtr();
		*((uint16_t*)cur_ptr) = m_usOP;
		cur_ptr += 2;
		*((uint8_t*)cur_ptr) = m_ucStrType;
		cur_ptr += 1;
		*((uint32_t*)cur_ptr) = m_uiSPIndex;
		cur_ptr += 4;
		*((uint8_t*)cur_ptr) = m_ucStrDataType;
		cur_ptr += 1;
		memcpy(cur_ptr, m_msText.data(), m_msText.size() + 1);
		String::Encode(cur_ptr);
	}

	Rut::RxJson::JValue SetStr::Make(size_t nCodePage) const
	{
		Rut::RxJson::JValue json;
		json[L"Name"] = L"SetStr";
		json[L"OP"] = String::NumToStr(L"0x%04x", m_usOP);
		json[L"StrType"] = String::NumToStr(L"0x%02x", m_ucStrType);
		json[L"SPIndex"] = String::NumToStr(L"0x%08x", m_uiSPIndex);
		json[L"StrDataType"] = String::NumToStr(L"0x%02x", m_ucStrDataType);
		json[L"Text"] = String::LoadText(m_msText, nCodePage);
		return json;
	}

	constexpr uint16_t SetStr::GetOP() const
	{
		return m_usOP;
	}

	const size_t SetStr::GetSize() const
	{
		assert(m_usOP != 0);
		return sizeof(m_usOP) + sizeof(m_ucStrType) + sizeof(m_uiSPIndex) + sizeof(m_ucStrDataType) + m_msText.size() + 1;
	}

	std::wstring SetStr::GetText(size_t nCodePage) const
	{
		return String::LoadText(m_msText, nCodePage);
	}

	void SetStr::SetText(std::wstring_view wsText, size_t nCodePage)
	{
		m_msText = String::MakeText(wsText, nCodePage);
	}
}