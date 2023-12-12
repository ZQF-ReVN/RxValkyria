#pragma once
#include <vector>
#include <stdexcept>

#include "../../Rut/RxMem.h"
#include "../../Rut/RxStr.h"
#include "../../Rut/RxJson.h"


namespace Valkyria::SDT
{
	static std::string ValkyrisReadString(uint8_t* pStr)
	{
		std::string str;
		while (true)
		{
			uint8_t tmp_char = pStr++[0] ^ 0xFF;
			if (tmp_char == '\0') { break; }
			str.append(1, (char)tmp_char);
		}
		return str;
	}


	class SDT_HDR_Info
	{
	private:
		uint32_t m_uiHeaderSize = 0;
		uint32_t m_uiUnValue0 = 0;
		uint32_t m_uiUnValue1 = 0;
		uint32_t m_uiLabelCount = 0;
		uint32_t m_uiCheckDataRva = 0;

	public:
		SDT_HDR_Info()
		{

		}

		void Parse(uint8_t* pData)
		{
			uint32_t* tmp_ptr = (uint32_t*)pData;
			this->m_uiHeaderSize = tmp_ptr[0];
			this->m_uiUnValue0 = tmp_ptr[1];
			this->m_uiUnValue1 = tmp_ptr[2];
			this->m_uiLabelCount = tmp_ptr[3];
			this->m_uiCheckDataRva = tmp_ptr[4];
		}

	public:
		size_t GetSize() const
		{
			return 5 * 4;
		}

		size_t GetHeaderSize() const
		{
			return this->m_uiHeaderSize;
		}

		size_t GetLabelCount() const
		{
			return this->m_uiLabelCount;
		}
	};

	class SDT_Label_Entry
	{
	private:
		std::string m_msLabelName;
		uint32_t m_uiLableInCodeOffset = 0;

	public:
		SDT_Label_Entry()
		{

		}

		void Parse(uint8_t* pData)
		{
			this->m_msLabelName = ValkyrisReadString(pData);
			this->m_uiLableInCodeOffset = *(uint32_t*)(pData + this->m_msLabelName.size() + 1);
		}

	public:
		size_t GetSize() const
		{
			return 4 + this->m_msLabelName.size() + 1;
		}
	};

	class SDT_Label_Index
	{
	private:
		std::vector<SDT_Label_Entry> m_vcLabels;

	public:
		SDT_Label_Index()
		{

		}

		void Parse(uint8_t* pData, size_t uiLabelCount)
		{
			uint8_t* cur_ptr = pData;

			for (size_t ite_label = 0; ite_label < uiLabelCount; ite_label++)
			{
				SDT_Label_Entry label;
				label.Parse(cur_ptr);
				cur_ptr += label.GetSize();
				this->m_vcLabels.emplace_back(std::move(label));
			}
		}

	public:
		size_t GetSize() const
		{
			size_t size = 0;
			for (auto& label : this->m_vcLabels)
			{
				size += label.GetSize();
			}
			return size;
		}
	};

	class SDT_HDR
	{
	private:
		SDT_HDR_Info m_Info;
		SDT_Label_Index m_Labels;
		uint8_t m_aCheckData[9] = { 0 };

	public:
		SDT_HDR()
		{

		}

		void Parse(uint8_t* pData)
		{
			uint8_t* cur_ptr = pData;

			this->m_Info.Parse(cur_ptr);
			cur_ptr += m_Info.GetSize();

			this->m_Labels.Parse(cur_ptr, this->m_Info.GetLabelCount());
			cur_ptr += this->m_Labels.GetSize();

			memcpy(this->m_aCheckData, cur_ptr, sizeof(this->m_aCheckData));
		}

	public:
		size_t GetSize() const
		{
			return m_Info.GetHeaderSize();
		}
	};

	class SDT_Code_MsgName
	{
	private:
		uint16_t m_usOPCode = 0;
		std::string m_msText;

	public:
		SDT_Code_MsgName()
		{


		}

		bool Parse(uint8_t* pData)
		{
			uint16_t op = *((uint16_t*)pData + 0);
			if (op != 0x0E00) { return false; }

			m_usOPCode = op;
			m_msText = ValkyrisReadString(pData + 2);
			uint32_t check = *(uint32_t*)(pData + sizeof(m_usOPCode) + m_msText.size() + 1);
			check != 0x11110E01 ? (throw std::runtime_error("SDT_Code_MsgName::Parse Error!")) : (void)(0);
			return true;
		}

		std::string GetText()
		{
			return m_msText;
		}

		size_t GetSize() const
		{
			return m_usOPCode ? (sizeof(m_usOPCode) + m_msText.size() + 1) : (0);
		}
	};

	class SDT_Code_MsgText
	{
	private:
		uint16_t m_usOPCode = 0;
		uint16_t m_usUnValue = 0;
		uint32_t m_uiLineNumer = 0;
		std::string m_msText;

	public:
		SDT_Code_MsgText()
		{

		}

		void Parse(uint8_t* pData)
		{
			this->m_usOPCode = *((uint16_t*)pData + 0);
			this->m_usOPCode != 0x0E01 ? (throw std::runtime_error("SDT_Code_MsgText::Parse Error!")) : (void)(0);
			this->m_usUnValue = *((uint16_t*)pData + 1);
			this->m_usUnValue != 0x1111 ? (throw std::runtime_error("SDT_Code_MsgText::Parse Error!")) : (void)(0);
			this->m_uiLineNumer = *((uint32_t*)pData + 1);
			this->m_msText = ValkyrisReadString(pData + 8);
		}

	public:
		std::string GetText()
		{
			return m_msText;
		}

		size_t GetSize() const
		{
			return sizeof(this->m_usOPCode) + sizeof(this->m_usUnValue) + sizeof(this->m_uiLineNumer) + (this->m_msText.size() + 1);
		}
	};

	class SDT_Code_MsgNewLine
	{
	private:
		uint16_t m_usOPCode = 0;
		uint16_t m_usUnValue = 0;
		uint32_t m_uiLineNumer = 0;

	public:
		SDT_Code_MsgNewLine()
		{

		}

		bool Parse(uint8_t* pData)
		{
			uint16_t op = *((uint16_t*)pData + 0);
			if (op != 0x0E04) { return false; }

			this->m_usOPCode = op;
			this->m_usUnValue = *((uint16_t*)pData + 1);
			this->m_usUnValue != 0x1111 ? (throw std::runtime_error("SDT_Code_MsgNewLine::Parse Error!")) : (void)(0);
			this->m_uiLineNumer = *((uint32_t*)pData + 1);

			return true;
		}

		size_t GetSize() const
		{
			return m_usOPCode ? (sizeof(this->m_usOPCode) + sizeof(this->m_usUnValue) + sizeof(this->m_uiLineNumer)) : (0);
		}
	};

	class SDT_Code_SelectText
	{
	private:
		uint16_t m_usOPCode = 0;
		uint32_t m_usUnValue = 0;
		std::vector<std::string> m_vcText;

	public:
		SDT_Code_SelectText()
		{

		}

		void Parse(uint8_t* pData)
		{
			this->m_usOPCode = *((uint16_t*)pData + 0);
			this->m_usOPCode != 0x0E1C ? (throw std::runtime_error("SDT_Code_SelectText::Parse Error!")) : (void)(0);
			this->m_usUnValue = *((uint16_t*)pData + 1);
			this->m_usUnValue != 0x00000000 ? (throw std::runtime_error("SDT_Code_SelectText::Parse Error!")) : (void)(0);

			uint8_t* text_array_ptr = pData + 6;
			while (true)
			{
				uint8_t type = text_array_ptr[0];
				if (type == 0x8)
				{
					std::string text = ValkyrisReadString(text_array_ptr + 1);
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

		size_t GetSize() const
		{
			size_t size = 0;
			size += sizeof(m_usOPCode) + sizeof(m_usUnValue);

			for (auto& text : m_vcText)
			{
				size += 1 + text.size() + 1;
			}

			return size;
		}
	};


	class SDT_Parser
	{
	private:
		SDT_HDR m_HDR;
		Rut::RxMem::Auto m_amCode;

	public:
		SDT_Parser()
		{

		}

		SDT_Parser(std::wstring_view wsPath)
		{
			Rut::RxMem::Auto sdt_buffer(wsPath);
			this->Parse(sdt_buffer);
		}

		void Parse(Rut::RxMem::Auto& amSDT)
		{
			this->m_HDR.Parse(amSDT.GetPtr());

			size_t code_seg_size = amSDT.GetSize() - this->m_HDR.GetSize();
			uint8_t* code_seg_ptr = amSDT.GetPtr() + this->m_HDR.GetSize();
			this->m_amCode.SetSize(code_seg_size);

			memcpy(this->m_amCode.GetPtr(), code_seg_ptr, code_seg_size);
		}

	public:
		Rut::RxMem::Auto& GetCodeMem()
		{
			return this->m_amCode;
		}
	};

}