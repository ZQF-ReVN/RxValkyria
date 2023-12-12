#pragma once
#include "SDT_Parser.h"


namespace Valkyria::SDT
{
	class SDT_Custom_Msg
	{
	private:
		SDT_Code_MsgName m_Name;
		SDT_Code_MsgText m_Text;
		SDT_Code_MsgNewLine m_NewLine;

		size_t m_uiBegCodeOffset = 0;
		size_t m_uiEndCodeOffset = 0;

	public:
		SDT_Custom_Msg()
		{

		}

		void Parse(uint8_t* pCodeSeg, size_t uiCodeOffset)
		{
			m_uiBegCodeOffset = uiCodeOffset;

			uint8_t* cur_ptr = pCodeSeg + uiCodeOffset;

			m_Name.Parse(cur_ptr);
			cur_ptr += m_Name.GetSize();

			m_Text.Parse(cur_ptr);
			cur_ptr += m_Text.GetSize();

			m_NewLine.Parse(cur_ptr);
			cur_ptr += m_NewLine.GetSize();

			m_uiEndCodeOffset = (size_t)(cur_ptr - pCodeSeg);
		}

		Rut::RxJson::Value ToJson()
		{
			Rut::RxJson::Value json;
			Rut::RxJson::JObject& obj = json.ToOBJ();
			obj[L"Name"] = Rut::RxStr::ToWCS(m_Name.GetText(), 932);
			obj[L"Text"] = Rut::RxStr::ToWCS(m_Text.GetText(), 932);
			return json;
		}

		void MakeTable()
		{
			// To do make a table used to index GOTO command to return end of Msg Code Struct
		}

		void WriteTable()
		{

		}

	public:
		size_t GetSize() const
		{
			return m_Name.GetSize() + m_Text.GetSize() + m_NewLine.GetSize();
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
					ite_byte += msg_name_code.GetSize();
					m_vcMsgNameCodes.push_back(std::move(msg_name_code));
				}
				else if (memcmp(code_ptr + ite_byte, search_msg_text_code, sizeof(search_msg_text_code)) == 0)
				{
					SDT_Code_MsgText msg_text_code;
					msg_text_code.Parse(code_ptr + ite_byte);
					ite_byte += msg_text_code.GetSize();
					m_vcMsgTextCodes.emplace_back(std::move(msg_text_code));

					SDT_Code_MsgNewLine msg_newline_code;
					msg_newline_code.Parse(code_ptr + ite_byte);
					ite_byte += msg_newline_code.GetSize();
					m_vcMsgNewLineCodes.push_back(std::move(msg_newline_code));
				}
				else if (memcmp(code_ptr + ite_byte, search_select_text_code, sizeof(search_select_text_code)) == 0)
				{
					SDT_Code_SelectText select_text_code;
					select_text_code.Parse(code_ptr + ite_byte);
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

	class STD_Text
	{
	private:
		std::vector<SDT_Custom_Msg> m_vcMsg;
		std::vector<SDT_Code_SelectText> m_vcSelectTextCode;

	public:
		STD_Text()
		{

		}

		void Parse(Rut::RxMem::Auto& amCode)
		{
			uint8_t search_msg_text_code[4] = { 0x01, 0x0E, 0x11, 0x11 };
			uint8_t search_msg_name_code[4] = { 0x00, 0x0E, 0x7E, 0x86 };
			uint8_t search_select_text_code[6] = { 0x1C, 0x0E, 0x00, 0x00, 0x00, 0x00 };

			uint8_t* code_ptr = amCode.GetPtr();
			size_t max_search_size = amCode.GetSize() - sizeof(search_select_text_code);
			for (size_t ite_byte = 0; ite_byte < max_search_size;)
			{
				if ((memcmp(code_ptr + ite_byte, search_msg_name_code, sizeof(search_msg_name_code)) == 0) ||
					(memcmp(code_ptr + ite_byte, search_msg_text_code, sizeof(search_msg_text_code)) == 0))
				{
					SDT_Custom_Msg msg;
					msg.Parse(code_ptr, ite_byte);
					ite_byte += msg.GetSize();
					m_vcMsg.push_back(std::move(msg));
				}
				else if (memcmp(code_ptr + ite_byte, search_select_text_code, sizeof(search_select_text_code)) == 0)
				{
					SDT_Code_SelectText select_text_code;
					select_text_code.Parse(code_ptr + ite_byte);
					ite_byte += select_text_code.GetSize();
					m_vcSelectTextCode.push_back(std::move(select_text_code));
				}
				else
				{
					ite_byte++;
				}
			}
		}

		Rut::RxJson::Value ToJson()
		{
			Rut::RxJson::Value json;

			for (auto& msg : m_vcMsg)
			{
				json.Append(msg.ToJson());
			}

			return json;
		}

		size_t GetMsgCount() const
		{
			return m_vcMsg.size();
		}
	};
}