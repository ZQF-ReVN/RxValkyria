#include "SDT_Code.h"
#include "SDT_String.h"


namespace Valkyria::SDT::Code
{
	MsgName::MsgName()
	{

	}

	MsgName::MsgName(uint8_t* pData)
	{
		this->Parse(pData);
	}

	void MsgName::Parse(uint8_t* pData)
	{
		m_usOPCode = *((uint16_t*)pData + 0);
		m_usOPCode != 0x0E00 ? (throw std::runtime_error("SDT_Code_MsgName::Parse Error!")) : (void)(0);
		m_msText = String::Decode(pData + 2);
		uint32_t check = *(uint32_t*)(pData + sizeof(m_usOPCode) + m_msText.size() + 1);
		check != 0x11110E01 ? (throw std::runtime_error("SDT_Code_MsgName::Parse Error!")) : (void)(0);
	}

	Rut::RxMem::Auto MsgName::Make() const
	{
		Rut::RxMem::Auto mem_data;
		size_t mem_size = this->GetSize();
		if (mem_size)
		{
			mem_data.SetSize(mem_size);
			uint8_t* cur_ptr = mem_data.GetPtr();

			(*(uint16_t*)cur_ptr) = m_usOPCode;
			cur_ptr += sizeof(m_usOPCode);

			memcpy(cur_ptr, m_msText.data(), m_msText.size() + 1);
			String::Encode(cur_ptr);
		}
		return mem_data;
	}

	void MsgName::SetText(std::string msText)
	{
		m_msText = msText;
	}

	const std::string& MsgName::GetText() const
	{
		return m_msText;
	}

	constexpr uint16_t MsgName::GetOPCode() const
	{
		return m_usOPCode;
	}

	constexpr size_t MsgName::GetSize() const
	{
		return m_usOPCode ? (sizeof(m_usOPCode) + m_msText.size() + 1) : (0);
	}


	MsgText::MsgText()
	{

	}

	MsgText::MsgText(uint8_t* pData)
	{
		this->Parse(pData);
	}

	void MsgText::Parse(uint8_t* pData)
	{
		m_usOPCode = *((uint16_t*)pData + 0);
		m_usOPCode != 0x0E01 ? (throw std::runtime_error("SDT_Code_MsgText::Parse Error!")) : (void)(0);
		m_usUnValue = *((uint16_t*)pData + 1);
		m_usUnValue != 0x1111 ? (throw std::runtime_error("SDT_Code_MsgText::Parse Error!")) : (void)(0);
		m_uiLineNumer = *((uint32_t*)pData + 1);
		m_msText = String::Decode(pData + 8);
	}

	Rut::RxMem::Auto MsgText::Make() const
	{
		Rut::RxMem::Auto mem_data;
		size_t mem_size = this->GetSize();
		if (mem_size)
		{
			mem_data.SetSize(mem_size);
			uint8_t* cur_ptr = mem_data.GetPtr();

			(*(uint16_t*)cur_ptr) = m_usOPCode;
			cur_ptr += sizeof(m_usOPCode);

			(*(uint16_t*)cur_ptr) = m_usUnValue;
			cur_ptr += sizeof(m_usUnValue);

			(*(uint32_t*)cur_ptr) = m_uiLineNumer;
			cur_ptr += sizeof(m_uiLineNumer);

			memcpy(cur_ptr, m_msText.data(), m_msText.size() + 1);
			String::Encode(cur_ptr);
		}
		return mem_data;
	}

	void MsgText::SetText(std::string msText)
	{
		m_msText = msText;
	}

	const std::string& MsgText::GetText() const
	{
		return m_msText;
	}

	constexpr uint16_t MsgText::GetOPCode() const
	{
		return m_usOPCode;
	}

	constexpr size_t MsgText::GetSize() const
	{
		return m_usOPCode ? (sizeof(m_usOPCode) + sizeof(m_usUnValue) + sizeof(m_uiLineNumer) + (m_msText.size() + 1)) : (0);
	}


	MsgNewLine::MsgNewLine()
	{

	}

	MsgNewLine::MsgNewLine(uint8_t* pData)
	{
		this->Parse(pData);
	}

	void MsgNewLine::Parse(uint8_t* pData)
	{
		m_usOPCode = *((uint16_t*)pData + 0);
		m_usOPCode != 0x0E04 ? (throw std::runtime_error("SDT_Code_MsgNewLine::Parse Error!")) : (void)(0);
		m_usUnValue = *((uint16_t*)pData + 1);
		m_usUnValue != 0x1111 ? (throw std::runtime_error("SDT_Code_MsgNewLine::Parse Error!")) : (void)(0);
		m_uiLineNumer = *((uint32_t*)pData + 1);
	}

	Rut::RxMem::Auto MsgNewLine::Make() const
	{
		Rut::RxMem::Auto mem_data;
		size_t mem_size = this->GetSize();
		if (mem_size)
		{
			mem_data.SetSize(mem_size);
			uint8_t* cur_ptr = mem_data.GetPtr();

			(*(uint16_t*)cur_ptr) = m_usOPCode;
			cur_ptr += sizeof(m_usOPCode);

			(*(uint16_t*)cur_ptr) = m_usUnValue;
			cur_ptr += sizeof(m_usUnValue);

			(*(uint32_t*)cur_ptr) = m_uiLineNumer;
			cur_ptr += sizeof(m_uiLineNumer);
		}
		return mem_data;
	}

	constexpr uint16_t MsgNewLine::GetOPCode() const
	{
		return m_usOPCode;
	}

	constexpr size_t MsgNewLine::GetSize() const
	{
		return m_usOPCode ? (sizeof(m_usOPCode) + sizeof(m_usUnValue) + sizeof(m_uiLineNumer)) : (0);
	}


	SelectText::SelectText()
	{

	}

	SelectText::SelectText(uint8_t* pData)
	{
		this->Parse(pData);
	}

	void SelectText::Parse(uint8_t* pData)
	{
		m_usOPCode = *((uint16_t*)pData + 0);
		m_usOPCode != 0x0E1C ? (throw std::runtime_error("SDT_Code_SelectText::Parse Error!")) : (void)(0);
		m_usUnValue = *((uint16_t*)pData + 1);
		m_usUnValue != 0x00000000 ? (throw std::runtime_error("SDT_Code_SelectText::Parse Error!")) : (void)(0);

		uint8_t* text_array_ptr = pData + 6;
		while (true)
		{
			uint8_t type = text_array_ptr[0];
			if (type == 0x8)
			{
				std::string text = String::Decode(text_array_ptr + 1);
				m_vcText.push_back(text);
				text_array_ptr += sizeof(type) + text.size() + 1;
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

	Rut::RxMem::Auto SelectText::Make() const
	{
		Rut::RxMem::Auto mem_data;
		size_t mem_size = this->GetSize();
		if (mem_size)
		{
			mem_data.SetSize(mem_size);
			uint8_t* cur_ptr = mem_data.GetPtr();

			(*(uint16_t*)cur_ptr) = m_usOPCode;
			cur_ptr += sizeof(m_usOPCode);

			(*(uint32_t*)cur_ptr) = m_usUnValue;
			cur_ptr += sizeof(m_usUnValue);

			for (auto& text : m_vcText)
			{
				*cur_ptr = 0x8; // Write Type
				memcpy(cur_ptr + 1, text.data(), text.size() + 1);
				String::Encode(cur_ptr + 1);
				cur_ptr += 1 + text.size() + 1;
			}

			*cur_ptr = 0xFF; // End Array
		}
		return mem_data;
	}

	void SelectText::SetTexts(std::vector<std::string>& vcText)
	{
		if (vcText.size() != m_vcText.size()) { throw std::runtime_error("SDT_Code_SelectText::SetTexts Error!"); }
		std::swap(m_vcText, vcText);
	}


	const std::vector<std::string>& SelectText::GetTexts() const
	{
		return m_vcText;
	}

	constexpr uint16_t SelectText::GetOPCode() const
	{
		return m_usOPCode;
	}

	constexpr size_t SelectText::GetSize() const
	{
		if (m_usOPCode == 0) { return 0; }

		size_t size = 0;
		size += sizeof(m_usOPCode) + sizeof(m_usUnValue);

		for (auto& text : m_vcText)
		{
			size += 1 + text.size() + 1;
		}

		size += 0x1; // end array flag

		return size;
	}


}