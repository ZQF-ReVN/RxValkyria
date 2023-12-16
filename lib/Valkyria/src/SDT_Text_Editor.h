#pragma once
#include "SDT_Parser.h"


namespace Valkyria::SDT
{
	static std::wstring NumToStr(size_t nValue)
	{
		wchar_t tmp[0x10];
		int len = swprintf_s(tmp, 0x10, L"0x%08x", nValue);
		return { tmp, (size_t)len };
	}

	class SDT_Custom_Msg
	{
	private:
		SDT_Code_MsgName m_Name;
		SDT_Code_MsgText m_Text;
		SDT_Code_MsgNewLine m_NewLine;
		SDT_Code_SelectText m_Select;

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

			uint16_t op = *((uint16_t*)cur_ptr);

			if (op == 0x0E00)
			{
				m_Name.Parse(cur_ptr);
				cur_ptr += m_Name.GetSize();
				op = *((uint16_t*)cur_ptr);
			}

			if (op == 0x0E01)
			{
				m_Text.Parse(cur_ptr);
				cur_ptr += m_Text.GetSize();
				op = *((uint16_t*)cur_ptr);
			}

			if (op == 0x0E04)
			{
				m_NewLine.Parse(cur_ptr);
				cur_ptr += m_NewLine.GetSize();
				op = *((uint16_t*)cur_ptr);
			}
			
			if (op == 0x0E1C)
			{
				m_Select.Parse(cur_ptr);
				cur_ptr += m_Select.GetSize();
				op = *((uint16_t*)cur_ptr);
			}

			m_uiEndCodeOffset = (size_t)(cur_ptr - pCodeSeg);
		}


		Rut::RxMem Make() const
		{
			return m_Name.Make() + m_Text.Make() + m_NewLine.Make() + m_Select.Make();
		}

		Rut::RxJson::Value ToJson() const
		{
			Rut::RxJson::Value json;
			Rut::RxJson::JObject& obj = json.ToOBJ();

			const std::string& name_text = m_Name.GetText();
			if (name_text.size()) 
			{ 
				obj[L"Name"] = Rut::RxStr::ToWCS(name_text, 932);
			}

			const std::string& msg_text = m_Text.GetText();
			if (msg_text.size()) 
			{ 
				obj[L"Text"] = Rut::RxStr::ToWCS(msg_text, 932);
			}

			const std::vector<std::string>& select_texts = m_Select.GetTexts();
			if (select_texts.size())
			{
				Rut::RxJson::Value& json_selects = obj[L"Select"];
				for (auto& text : select_texts)
				{
					json_selects.Append(Rut::RxStr::ToWCS(text, 932));
				}
			}

			// Debug
			if (true)
			{
				obj[L"Debug_BegCodeOffset"] = NumToStr(m_uiBegCodeOffset);
				obj[L"Debug_EndCodeOffset"] = NumToStr(m_uiEndCodeOffset);
			}

			return json;
		}

		void LoadViaJson(Rut::RxJson::Value& rfJson, size_t nCodePage)
		{
			if (rfJson.HasKey(L"Name"))
			{
				m_Name.SetText(Rut::RxStr::ToMBCS(rfJson[L"Name"], nCodePage));
			}

			if (rfJson.HasKey(L"Text"))
			{
				m_Text.SetText(Rut::RxStr::ToMBCS(rfJson[L"Text"], nCodePage));
			}

			if (rfJson.HasKey(L"Select"))
			{
				std::vector<std::string> select_texts;
				Rut::RxJson::JArray selects_json = rfJson[L"Select"].ToAry();
				for (auto& select : selects_json)
				{
					select_texts.push_back(Rut::RxStr::ToMBCS(select.ToStringView(), nCodePage));
				}
				m_Select.SetTexts(select_texts);
			}
		}

	public:
		size_t GetSize() const
		{
			return m_Name.GetSize() + m_Text.GetSize() + m_NewLine.GetSize() + m_Select.GetSize();
		}
	};

	template<typename SDT_OBJ_T>
	void CheckMakeData(uint8_t* pOrg, SDT_OBJ_T& rfOBJ)
	{
		Rut::RxMem mem = rfOBJ.Make();
		if (memcmp(pOrg, mem.GetPtr(), mem.GetSize()))
		{
			throw std::runtime_error("Dump Data Mismatched!");
		}
	}

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

		void Parse(Rut::RxMem& amCode)
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

	class STD_Text
	{
	private:
		SDT_Parser m_SDT;
		std::vector<SDT_Custom_Msg> m_vcMsg;

	public:
		STD_Text()
		{

		}

		STD_Text(std::wstring_view wsPath)
		{
			m_SDT.Parse(wsPath);
		}

		void Parse()
		{
			if (m_vcMsg.size()) { m_vcMsg.clear(); }

			Rut::RxMem& code_mem = m_SDT.GetCodeMem();
			if (code_mem.GetSize() < 0x10) { return; }

			uint8_t search_msg_text_code[4] = { 0x01, 0x0E, 0x11, 0x11 };
			uint8_t search_msg_name_code[4] = { 0x00, 0x0E, 0x7E, 0x86 };
			uint8_t search_select_text_code[6] = { 0x1C, 0x0E, 0x00, 0x00, 0x00, 0x00 };

			uint8_t* code_ptr = code_mem.GetPtr();
			size_t max_search_size = code_mem.GetSize() - sizeof(search_select_text_code);
			for (size_t ite_byte = 0; ite_byte < max_search_size;)
			{
				if ((memcmp(code_ptr + ite_byte, search_msg_name_code, sizeof(search_msg_name_code)) == 0) ||
					(memcmp(code_ptr + ite_byte, search_msg_text_code, sizeof(search_msg_text_code)) == 0) ||
					(memcmp(code_ptr + ite_byte, search_select_text_code, sizeof(search_select_text_code)) == 0))
				{
					SDT_Custom_Msg msg;
					msg.Parse(code_ptr, ite_byte);
					ite_byte += msg.GetSize();
					m_vcMsg.push_back(std::move(msg));
				}
				else
				{
					ite_byte++;
				}
			}
		}

		void Make()
		{

		}

		Rut::RxJson::Value ToJson() const
		{
			Rut::RxJson::Value json;

			for (auto& msg : m_vcMsg)
			{
				json.Append(msg.ToJson());
			}

			return json;
		}

		void LoadViaJson(std::wstring_view wsJsonPath, size_t nCodePage)
		{
			Rut::RxJson::Value json;
			Rut::RxJson::Parser parser(wsJsonPath, json);
			Rut::RxJson::JArray& json_msg_list = json.ToAry();

			if (m_vcMsg.size() != json_msg_list.size())
			{
				throw std::runtime_error("STD_Text::LoadViaJson Json Error!");
			}
		
			for (size_t ite = 0; ite < m_vcMsg.size(); ite++)
			{
				m_vcMsg[ite].LoadViaJson(json_msg_list[ite], nCodePage);
			}
		}

		constexpr size_t GetMsgCount() const
		{
			return m_vcMsg.size();
		}
	};
}