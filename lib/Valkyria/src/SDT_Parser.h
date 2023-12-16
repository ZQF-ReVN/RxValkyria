#pragma once
#include <vector>
#include <stdexcept>

#include "../../Rut/RxMem.h"
#include "../../Rut/RxStr.h"
#include "../../Rut/RxJson.h"


namespace Valkyria::SDT
{
	static std::string ValkyriaStringDecode(uint8_t* pStr)
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

	static void ValkyriaStringEncode(uint8_t* pStr)
	{
		while (true)
		{
			uint8_t tmp_char = pStr[0] ^ 0xFF;
			pStr[0] = tmp_char;
			if (tmp_char == 0xFF) { break; }
			pStr++;
		}
	}

	template<typename SDT_OBJ_T>
	static void CheckMakeData(uint8_t* pOrg, SDT_OBJ_T& rfOBJ)
	{
		Rut::RxMem::Auto mem = rfOBJ.Make();
		if (memcmp(pOrg, mem.GetPtr(), mem.GetSize()))
		{
			throw std::runtime_error("Dump Data Mismatched!");
		}
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
			m_uiHeaderSize = tmp_ptr[0];
			m_uiUnValue0 = tmp_ptr[1];
			m_uiUnValue1 = tmp_ptr[2];
			m_uiLabelCount = tmp_ptr[3];
			m_uiCheckDataRva = tmp_ptr[4];
		}

		Rut::RxMem::Auto Make() const
		{
			Rut::RxMem::Auto mem_data;
			mem_data.SetSize(this->GetSize());
			uint32_t* cur_ptr = (uint32_t*)mem_data.GetPtr();
			{
				cur_ptr[0] = m_uiHeaderSize;
				cur_ptr[1] = m_uiUnValue0;
				cur_ptr[2] = m_uiUnValue1;
				cur_ptr[3] = m_uiLabelCount;
				cur_ptr[4] = m_uiCheckDataRva;
			}
			return mem_data;
		}

	public:
		size_t GetSize() const
		{
			return sizeof(m_uiHeaderSize) + sizeof(m_uiUnValue0) + sizeof(m_uiUnValue1) + sizeof(m_uiLabelCount) + sizeof(m_uiCheckDataRva);
		}

		size_t GetHeaderSize() const
		{
			return m_uiHeaderSize;
		}

		size_t GetLabelCount() const
		{
			return m_uiLabelCount;
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
			m_msLabelName = ValkyriaStringDecode(pData);
			m_uiLableInCodeOffset = *(uint32_t*)(pData + m_msLabelName.size() + 1);
		}

		Rut::RxMem::Auto Make() const
		{
			Rut::RxMem::Auto mem_data;
			mem_data.SetSize(this->GetSize());
			uint8_t* cur_ptr = mem_data.GetPtr();
			{
				memcpy(cur_ptr, m_msLabelName.data(), m_msLabelName.size() + 1);
				ValkyriaStringEncode(cur_ptr);
				cur_ptr += m_msLabelName.size() + 1;
				*(uint32_t*)cur_ptr = m_uiLableInCodeOffset;
			}
			return mem_data;
		}

	public:
		size_t GetSize() const
		{
			return m_msLabelName.size() + 1 + sizeof(m_uiLableInCodeOffset);
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
				m_vcLabels.emplace_back(std::move(label));
			}
		}

		Rut::RxMem::Auto Make() const
		{
			Rut::RxMem::Auto mem_data;
			mem_data.SetSize(this->GetSize());
			uint8_t* cur_ptr = mem_data.GetPtr();
			{
				for (auto& label : m_vcLabels)
				{
					Rut::RxMem::Auto label_mem = label.Make();
					memcpy(cur_ptr, label_mem.GetPtr(), label_mem.GetSize());
					cur_ptr += label_mem.GetSize();
				}
			}
			return mem_data;
		}

	public:
		size_t GetSize() const
		{
			size_t size = 0;
			for (auto& label : m_vcLabels)
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

			m_Info.Parse(cur_ptr);
			cur_ptr += m_Info.GetSize();

			m_Labels.Parse(cur_ptr, m_Info.GetLabelCount());
			cur_ptr += m_Labels.GetSize();

			memcpy(m_aCheckData, cur_ptr, sizeof(m_aCheckData));
		}

		Rut::RxMem::Auto Make() const
		{
			Rut::RxMem::Auto mem_data;
			mem_data.SetSize(this->GetSize());
			uint8_t* cur_ptr = mem_data.GetPtr();

			Rut::RxMem::Auto info_mem = m_Info.Make();
			memcpy(cur_ptr, info_mem.GetPtr(), info_mem.GetSize());
			cur_ptr += info_mem.GetSize();

			Rut::RxMem::Auto labels_mem = m_Labels.Make();
			memcpy(cur_ptr, labels_mem.GetPtr(), labels_mem.GetSize());
			cur_ptr += labels_mem.GetSize();

			memcpy(cur_ptr, m_aCheckData, sizeof(m_aCheckData));
			
			return mem_data;
		}

	public:
		size_t GetSize() const
		{
			return m_Info.GetHeaderSize();
		}

		const SDT_HDR_Info& GetInfo() const
		{
			return m_Info;
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

		SDT_Code_MsgName(uint8_t* pData)
		{
			this->Parse(pData);
		}

		void Parse(uint8_t* pData)
		{
			m_usOPCode = *((uint16_t*)pData + 0);
			m_usOPCode != 0x0E00 ? (throw std::runtime_error("SDT_Code_MsgName::Parse Error!")) : (void)(0);
			m_msText = ValkyriaStringDecode(pData + 2);
			uint32_t check = *(uint32_t*)(pData + sizeof(m_usOPCode) + m_msText.size() + 1);
			check != 0x11110E01 ? (throw std::runtime_error("SDT_Code_MsgName::Parse Error!")) : (void)(0);
		}

		Rut::RxMem::Auto Make() const
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
				ValkyriaStringEncode(cur_ptr);
			}
			return mem_data;
		}

	public:
		void SetText(std::string msText)
		{
			m_msText = msText;
		}

		const std::string& GetText() const
		{
			return m_msText;
		}

		uint16_t GetOPCode() const
		{
			return m_usOPCode;
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

		SDT_Code_MsgText(uint8_t* pData)
		{
			this->Parse(pData);
		}

		void Parse(uint8_t* pData)
		{
			m_usOPCode = *((uint16_t*)pData + 0);
			m_usOPCode != 0x0E01 ? (throw std::runtime_error("SDT_Code_MsgText::Parse Error!")) : (void)(0);
			m_usUnValue = *((uint16_t*)pData + 1);
			m_usUnValue != 0x1111 ? (throw std::runtime_error("SDT_Code_MsgText::Parse Error!")) : (void)(0);
			m_uiLineNumer = *((uint32_t*)pData + 1);
			m_msText = ValkyriaStringDecode(pData + 8);
		}

		Rut::RxMem::Auto Make() const
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
				ValkyriaStringEncode(cur_ptr);
			}
			return mem_data;
		}

	public:
		void SetText(std::string msText)
		{
			m_msText = msText;
		}

		const std::string& GetText() const
		{
			return m_msText;
		}

		uint16_t GetOPCode() const
		{
			return m_usOPCode;
		}

		size_t GetSize() const
		{
			return m_usOPCode ? (sizeof(m_usOPCode) + sizeof(m_usUnValue) + sizeof(m_uiLineNumer) + (m_msText.size() + 1)) : (0);
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

		SDT_Code_MsgNewLine(uint8_t* pData)
		{
			this->Parse(pData);
		}

		void Parse(uint8_t* pData)
		{
			m_usOPCode = *((uint16_t*)pData + 0);
			m_usOPCode != 0x0E04 ? (throw std::runtime_error("SDT_Code_MsgNewLine::Parse Error!")) : (void)(0);
			m_usUnValue = *((uint16_t*)pData + 1);
			m_usUnValue != 0x1111 ? (throw std::runtime_error("SDT_Code_MsgNewLine::Parse Error!")) : (void)(0);
			m_uiLineNumer = *((uint32_t*)pData + 1);
		}

		Rut::RxMem::Auto Make() const
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

	public:
		uint16_t GetOPCode() const
		{
			return m_usOPCode;
		}

		size_t GetSize() const
		{
			return m_usOPCode ? (sizeof(m_usOPCode) + sizeof(m_usUnValue) + sizeof(m_uiLineNumer)) : (0);
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

		SDT_Code_SelectText(uint8_t* pData)
		{
			this->Parse(pData);
		}

		void Parse(uint8_t* pData)
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
					std::string text = ValkyriaStringDecode(text_array_ptr + 1);
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

		Rut::RxMem::Auto Make() const
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
					ValkyriaStringEncode(cur_ptr + 1);
					cur_ptr += 1 + text.size() + 1;
				}

				*cur_ptr = 0xFF; // End Array
			}
			return mem_data;
		}

	public:
		void SetTexts(std::vector<std::string>& vcText)
		{
			if (vcText.size() != m_vcText.size()) { throw std::runtime_error("SDT_Code_SelectText::SetTexts Error!"); }
			std::swap(m_vcText, vcText);
		}


		const std::vector<std::string>& GetTexts() const
		{
			return m_vcText;
		}

		uint16_t GetOPCode() const
		{
			return m_usOPCode;
		}

		size_t GetSize() const
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
			this->Parse(wsPath);
		}

		void Parse(std::wstring_view wsPath)
		{
			Rut::RxMem::Auto sdt_buffer(wsPath);
			this->Parse(sdt_buffer);
		}

		void Parse(Rut::RxMem::Auto& amSDT)
		{
			m_HDR.Parse(amSDT.GetPtr());

			size_t code_seg_size = amSDT.GetSize() - m_HDR.GetSize();
			uint8_t* code_seg_ptr = amSDT.GetPtr() + m_HDR.GetSize();
			m_amCode.SetSize(code_seg_size);

			memcpy(m_amCode.GetPtr(), code_seg_ptr, code_seg_size);
		}

		Rut::RxMem::Auto Make() const
		{
			return { m_HDR.Make(), m_amCode };
		}

	public:
		Rut::RxMem::Auto& GetCodeMem()
		{
			return m_amCode;
		}

		const SDT_HDR& GetHDR()
		{
			return m_HDR;
		}
	};


	class STD_Text_Test
	{
	private:
		std::vector<SDT_Code_MsgName> m_vcMsgNameCodes;
		std::vector<SDT_Code_MsgText> m_vcMsgTextCodes;
		std::vector<SDT_Code_MsgNewLine> m_vcMsgNewLineCodes;
		std::vector<SDT_Code_SelectText> m_vcSelectTextCode;

	public:
		STD_Text_Test()
		{

		}

		void Parse(Rut::RxMem::Auto& amCode)
		{
			if (amCode.GetSize() < 0x10) { return; }

			uint8_t search_msg_text_code[4] = { 0x01, 0x0E, 0x11, 0x11 };
			uint8_t search_msg_name_code[4] = { 0x00, 0x0E, 0x7E, 0x86 };
			uint8_t search_select_text_code[6] = { 0x1C, 0x0E, 0x00, 0x00, 0x00, 0x00 };
			uint8_t* code_ptr = amCode.GetPtr();
			size_t max_search_size = amCode.GetSize() - sizeof(search_select_text_code);

			for (size_t ite_byte = 0; ite_byte < max_search_size;)
			{
				uint8_t* cur_ptr = code_ptr + ite_byte;
				if (memcmp(code_ptr + ite_byte, search_msg_name_code, sizeof(search_msg_name_code)) == 0)
				{
					SDT_Code_MsgName msg_name_code;
					msg_name_code.Parse(code_ptr + ite_byte);
					{
						CheckMakeData(code_ptr + ite_byte, msg_name_code);
					}
					ite_byte += msg_name_code.GetSize();
					m_vcMsgNameCodes.push_back(std::move(msg_name_code));
				}
				else if (memcmp(code_ptr + ite_byte, search_msg_text_code, sizeof(search_msg_text_code)) == 0)
				{
					SDT_Code_MsgText msg_text_code;
					msg_text_code.Parse(code_ptr + ite_byte);
					{
						CheckMakeData(code_ptr + ite_byte, msg_text_code);
					}
					ite_byte += msg_text_code.GetSize();
					m_vcMsgTextCodes.emplace_back(std::move(msg_text_code));

					if (*(uint16_t*)(code_ptr + ite_byte) == 0x0E04)
					{
						SDT_Code_MsgNewLine msg_newline_code;
						msg_newline_code.Parse(code_ptr + ite_byte);
						{
							CheckMakeData(code_ptr + ite_byte, msg_newline_code);
						}
						ite_byte += msg_newline_code.GetSize();
						m_vcMsgNewLineCodes.push_back(std::move(msg_newline_code));
					}
				}
				else if (memcmp(code_ptr + ite_byte, search_select_text_code, sizeof(search_select_text_code)) == 0)
				{
					SDT_Code_SelectText select_text_code;
					select_text_code.Parse(code_ptr + ite_byte);
					{
						CheckMakeData(code_ptr + ite_byte, select_text_code);
					}
					ite_byte += select_text_code.GetSize();
					m_vcSelectTextCode.push_back(std::move(select_text_code));
				}
				else
				{
					ite_byte++;
				}
			}
		}
	};
}