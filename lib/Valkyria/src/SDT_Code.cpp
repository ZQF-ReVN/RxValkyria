#include "SDT_Code.h"
#include "SDT_String.h"

#include <cassert>


namespace Valkyria::SDT::Code
{
	MsgName::MsgName()
	{

	}

	MsgName::MsgName(uint8_t* const pData)
	{
		this->Load(pData);
	}

	void MsgName::Load(uint8_t* const pData)
	{
		m_usOP = *((uint16_t*)pData + 0);
		assert(m_usOP == 0x0E00);
		m_msText = String::Decode(pData + sizeof(m_usOP));
		assert(*(uint32_t*)(pData + sizeof(m_usOP) + m_msText.size() + 1) == 0x11110E01);
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


	MsgText::MsgText()
	{

	}

	MsgText::MsgText(uint8_t* const pData)
	{
		this->Load(pData);
	}

	void MsgText::Load(uint8_t* const pData)
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


	MsgNewLine::MsgNewLine()
	{

	}

	MsgNewLine::MsgNewLine(uint8_t* const pData)
	{
		this->Load(pData);
	}

	void MsgNewLine::Load(uint8_t* const pData)
	{
		m_usOP = *((uint16_t*)pData + 0);
		assert(m_usOP == 0x0E04);
		m_usUnknow = *((uint16_t*)pData + 1);
		assert(m_usUnknow == 0x1111);
		m_uiLineNumber = *((uint32_t*)pData + 1);
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

	SelectText::SelectText(uint8_t* const pData)
	{
		this->Load(pData);
	}

	void SelectText::Load(uint8_t* const pData)
	{
		m_usOP = *((uint16_t*)pData + 0);
		assert(m_usOP == 0x0E1C);
		m_usUnknow = *((uint16_t*)pData + 1);
		assert(m_usUnknow == 0x00000000);

		uint8_t* text_array_ptr = pData + 6;
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
		m_usUnknow = (uint16_t)String::StrToNum(L"0x%04x", rfJson[L"Unknow"]);

		m_vcText.clear();
		Rut::RxJson::JArray& text_list = rfJson[L"Texts"].ToAry();
		for (auto& text : text_list)
		{
			m_vcText.push_back(String::MakeText(text.ToStrView(), nCodePage));
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
		json[L"Unknow"] = String::NumToStr(L"0x%04x", m_usUnknow);

		Rut::RxJson::JValue& j_text_array = json[L"Texts"];
		for (auto& text : m_vcText) 
		{ 
			j_text_array.Append(String::LoadText(text, nCodePage));
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


}