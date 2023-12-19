#include "SDT_Text_Parser.h"
#include "../../Rut/RxStr.h"


namespace Valkyria::SDT
{
	static std::wstring NumToStr(size_t nValue)
	{
		wchar_t tmp[0x10];
		size_t len = (size_t)swprintf_s(tmp, 0x10, L"0x%08x", nValue);
		return { tmp, len };
	}

	static std::string MakeUnicodeStrA(size_t wChar)
	{
		char buf[0x10];
		size_t len = (size_t)sprintf_s(buf, 0x10, "\\u%04x", wChar);
		return { buf, len };
	}

	static  std::wstring MakeUnicodeStrW(size_t wChar)
	{
		wchar_t buf[0x10];
		size_t len = (size_t)swprintf_s(buf, 0x10, L"\\u%04x", wChar);
		return { buf, len };
	}

	static std::string EncodeString(std::wstring_view wsText, size_t nCodePage)
	{
		std::string str_bytes;

		if (nCodePage == 1200)
		{

			for (size_t ite_unit = 0; ite_unit < wsText.size(); ite_unit++)
			{
				wchar_t unit = wsText[ite_unit];
				if (unit == L'\\')
				{
					ite_unit++;
					unit = wsText[ite_unit];
					switch (unit)
					{
					case L'U':
					case L'u':
					{
						uint32_t code_point = 0;
						swscanf_s(wsText.data() + ite_unit + 1, L"%04x", &code_point);
						str_bytes.append(MakeUnicodeStrA(code_point));
						ite_unit += 4;
					}
					break;

					case L'R':
					case L'r':
					{
						str_bytes.append(1, '\\');
						str_bytes.append(1, 'r');
					}
					break;

					case L'\\':
					{
						throw std::runtime_error("Unknow Format");
					}
					break;

					default: throw std::runtime_error("Unknow Format");
					}
				}
				else
				{
					str_bytes.append(MakeUnicodeStrA(unit));
				}
			}
		}
		else
		{
			std::wstring format_text;
			for (auto& unit : wsText)
			{
				switch (unit)
				{
				case L'・':case L'≪':case L'≫':case L'♪':format_text.append(MakeUnicodeStrW(unit)); break;
				default: format_text.append(1, unit);
				}
			}
			str_bytes = Rut::RxStr::ToMBCS(format_text, nCodePage);
		}

		if (str_bytes.size() >= 260) { throw std::runtime_error("EncodeString: exceeds buffer size limit"); }
		return str_bytes;
	}



	Custom_Msg::Custom_Msg()
	{

	}

	Custom_Msg::Custom_Msg(uint8_t* pCodeSeg, size_t uiCodeOffset)
	{
		this->Parse(pCodeSeg, uiCodeOffset);
	}

	void Custom_Msg::Parse(uint8_t* pCodeSeg, size_t uiCodeOffset)
	{
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

		m_uiCodeBegOffset = uiCodeOffset;
		m_uiCodeEndOffset = (size_t)(cur_ptr - pCodeSeg);
	}


	Rut::RxMem::Auto Custom_Msg::Make() const
	{
		return { m_Name.Make(), m_Text.Make(), m_NewLine.Make(), m_Select.Make() };
	}

	Rut::RxJson::JValue Custom_Msg::ToJson(size_t nCodePage, bool isDebug, size_t uiHdrSize) const
	{
		Rut::RxJson::JValue json;
		Rut::RxJson::JObject& obj = json.ToOBJ();

		if (m_Name.GetText().size()) { obj[L"Name"] = Rut::RxStr::ToWCS(m_Name.GetText(), nCodePage); }
		if (m_Text.GetText().size()) { obj[L"Text"] = Rut::RxStr::ToWCS(m_Text.GetText(), nCodePage); }

		if (m_Select.GetTexts().size())
		{
			Rut::RxJson::JValue& json_selects = obj[L"Select"];
			for (auto& text : m_Select.GetTexts())
			{
				json_selects.Append(Rut::RxStr::ToWCS(text, nCodePage));
			}
		}

		if (isDebug)
		{
			obj[L"Debug_BegFOA"] = NumToStr(uiHdrSize + m_uiCodeBegOffset);
			obj[L"Debug_EndFOA"] = NumToStr(uiHdrSize + m_uiCodeEndOffset);
		}

		return json;
	}

	void Custom_Msg::Load(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		Rut::RxJson::JObject::iterator ite_name = rfJson.FindKey(L"Name");
		if (ite_name != rfJson.EndKey())
		{
			m_Name.SetText(EncodeString(rfJson.GetValue(ite_name).ToStringView(), nCodePage));
		}

		Rut::RxJson::JObject::iterator ite_text = rfJson.FindKey(L"Text");
		if (ite_text != rfJson.EndKey()) { m_Text.SetText(EncodeString(rfJson.GetValue(ite_text).ToStringView(), nCodePage)); }

		Rut::RxJson::JObject::iterator ite_select = rfJson.FindKey(L"Select");
		if (ite_select != rfJson.EndKey())
		{
			std::vector<std::string> select_texts;
			const Rut::RxJson::JArray& selects_json = rfJson.GetValue(ite_select).ToAry();
			for (auto& select : selects_json)
			{
				select_texts.push_back(EncodeString(select.ToStringView(), nCodePage));
			}
			m_Select.SetTexts(select_texts);
		}
	}

	constexpr size_t Custom_Msg::GetSize() const
	{
		return m_Name.GetSize() + m_Text.GetSize() + m_NewLine.GetSize() + m_Select.GetSize();
	}

	constexpr size_t Custom_Msg::GetCodeBegOffset() const
	{
		return m_uiCodeBegOffset;
	}

	constexpr size_t Custom_Msg::GetCodeEndOffset() const
	{
		return m_uiCodeEndOffset;
	}


	Text_Parser::Text_Parser()
	{

	}

	Text_Parser::Text_Parser(std::wstring_view wsPath)
	{
		this->Init(wsPath);
		this->Parse();
	}

	void Text_Parser::Init(std::wstring_view wsPath)
	{
		m_amSDT.LoadFile(wsPath);
		m_pInfo = (VAL_SDT_HDR_Info*)m_amSDT.GetPtr();
	}

	void Text_Parser::Parse()
	{
		if (m_vcMsg.size()) { m_vcMsg.clear(); }
		if (m_amSDT.GetSize() < 0x10) { return; }

		uint8_t search_msg_text_code[4] = { 0x01, 0x0E, 0x11, 0x11 };
		uint8_t search_msg_name_code[4] = { 0x00, 0x0E, 0x7E, 0x86 };
		uint8_t search_select_text_code[6] = { 0x1C, 0x0E, 0x00, 0x00, 0x00, 0x00 };

		uint8_t* code_ptr = m_amSDT.GetPtr() + m_pInfo->uiHDRSize;
		size_t code_size = m_amSDT.GetSize() - m_pInfo->uiHDRSize;
		size_t max_search_size = code_size - sizeof(search_select_text_code);

		for (size_t ite_byte = 0; ite_byte < max_search_size;)
		{
			if ((memcmp(code_ptr + ite_byte, search_msg_name_code, sizeof(search_msg_name_code)) == 0) ||
				(memcmp(code_ptr + ite_byte, search_msg_text_code, sizeof(search_msg_text_code)) == 0) ||
				(memcmp(code_ptr + ite_byte, search_select_text_code, sizeof(search_select_text_code)) == 0))
			{
				Custom_Msg msg(code_ptr, ite_byte);
				ite_byte += msg.GetSize();
				m_vcMsg.push_back(std::move(msg));
			}
			else
			{
				ite_byte++;
			}
		}
	}

	void Text_Parser::Load(Rut::RxJson::JArray& rfJarray, size_t nCodePage)
	{
		if (m_vcMsg.size() != rfJarray.size())
		{
			throw std::runtime_error("STD_Text::LoadViaJson Json Error!");
		}

		for (size_t ite = 0; ite < m_vcMsg.size(); ite++)
		{
			m_vcMsg[ite].Load(rfJarray[ite], nCodePage);
		}
	}

	Rut::RxMem::Auto Text_Parser::Make()
	{
		auto fn_count_append_size = [this]() -> size_t
			{
				size_t append_mem_size = 0;
				for (auto& msg : m_vcMsg)
				{
					append_mem_size += 4 + msg.GetSize() + 2 + 4 + 4;
				}
				return append_mem_size;
			};

		// Count Append Data Size
		Rut::RxMem::Auto append_mem;
		append_mem.SetSize(fn_count_append_size());

		size_t code_size = m_amSDT.GetSize() - m_pInfo->uiHDRSize;
		uint8_t* code_ptr = m_amSDT.GetPtr() + m_pInfo->uiHDRSize;
		size_t append_size = 0;
		uint8_t* append_ptr = append_mem.GetPtr();
		uint8_t goto_command[] = { 0x42, 0x0A, 0x00, 0x00, 0x00, 0x00 };

		for (auto& msg : m_vcMsg)
		{
			// Check if there is enough space to write goto commnad
			size_t free_space = msg.GetCodeEndOffset() - msg.GetCodeBegOffset();
			free_space < sizeof(goto_command) ? (throw std::runtime_error("Not enough space to write goto commnad")) : (void)(0);

			// write jmp [goto command]
			uint32_t target_code_offset_ptr_vale = m_pInfo->uiHDRSize + code_size + append_size - sizeof(VAL_SDT_HDR_Info);
			memcpy(goto_command + 2, &target_code_offset_ptr_vale, sizeof(uint32_t));
			memcpy(code_ptr + msg.GetCodeBegOffset(), goto_command, sizeof(goto_command));

			// write jmp [goto command] target offset
			uint32_t target_code_offset = code_size + append_size + 4;
			((uint32_t*)append_ptr)[0] = target_code_offset;
			append_ptr += 4;
			append_size += 4;

			// write msg code struct
			Rut::RxMem::Auto msg_data = msg.Make();
			memcpy(append_ptr, msg_data.GetPtr(), msg_data.GetSize());
			append_ptr += msg_data.GetSize();
			append_size += msg_data.GetSize();

			// write ret [goto command]
			uint32_t code_offset_ptr_vale_ret = m_pInfo->uiHDRSize + code_size + append_size + sizeof(goto_command) - sizeof(VAL_SDT_HDR_Info);
			memcpy(goto_command + 2, &code_offset_ptr_vale_ret, sizeof(uint32_t));
			memcpy(append_ptr, goto_command, sizeof(goto_command));
			append_ptr += sizeof(goto_command);
			append_size += sizeof(goto_command);

			// write ret [goto command] target offset
			((uint32_t*)append_ptr)[0] = msg.GetCodeEndOffset();
			append_ptr += 4;
			append_size += 4;
		}

		return { m_amSDT, append_mem };
	}

	Rut::RxJson::JValue Text_Parser::ToJson(size_t nCodePage, bool isDebug) const
	{
		Rut::RxJson::JValue json;

		for (auto& msg : m_vcMsg)
		{
			json.Append(msg.ToJson(nCodePage, isDebug, m_pInfo->uiHDRSize));
		}

		return json;
	}

	size_t Text_Parser::GetMsgCountViaScan() const
	{
		return m_vcMsg.size();
	}

	size_t Text_Parser::GetMsgCountViaInfo() const
	{
		return m_pInfo->uiMsgCount;
	}
}