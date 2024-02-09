#include "SDT_TextParser.h"
#include "SDT_String.h"
#include "SDT_Signer.h"


namespace Valkyria::SDT
{
	TextParser::TextParser()
	{

	}

	TextParser::TextParser(const std::filesystem::path& phSdt)
	{
		this->Read(phSdt);
	}

	void TextParser::Read(const std::filesystem::path& phSdt)
	{
		this->m_Sdt.Load(phSdt);
	}

	void TextParser::Scan()
	{
		if (m_vcMsg.size()) { m_vcMsg.clear(); }
		if (m_Sdt.GetSdtSize() < 0x10) { return; }

		uint8_t* code_ptr = m_Sdt.GetCodePtr();
		size_t code_size = m_Sdt.GetCodeSize();
		size_t max_search_size = code_size - 4;

		constexpr uint8_t const search_mask[][4] =
		{
			{ 0x01, 0x0E, 0x11, 0x11 }, // msg
			{ 0x04, 0x0E ,0x11, 0x11 }, // new line
			{ 0x00, 0x0E, 0x7E, 0x86 }, // msg name
			{ 0x1C, 0x0E, 0x00, 0x00 }, // select 
			{ 0x17, 0x0B, 0x09, 0x00 }, // setStr 
		};

		for (size_t ite_byte = 0; ite_byte < max_search_size;)
		{
			uint8_t* cur_ptr = code_ptr + ite_byte;
			if ((memcmp(cur_ptr, search_mask[0], sizeof(search_mask[0])) == 0) ||
				(memcmp(cur_ptr, search_mask[1], sizeof(search_mask[1])) == 0) ||
				(memcmp(cur_ptr, search_mask[2], sizeof(search_mask[2])) == 0) ||
				(memcmp(cur_ptr, search_mask[3], sizeof(search_mask[3])) == 0) ||
				(memcmp(cur_ptr, search_mask[4], sizeof(search_mask[4])) == 0))
			{
				TextCode msg(code_ptr, ite_byte);
				ite_byte += msg.GetSize();
				m_vcMsg.push_back(std::move(msg));
			}
			else
			{
				ite_byte++;
			}
		}
	}

	void TextParser::Load(Rut::RxJson::JArray& rfJarray, size_t nCodePage)
	{
		if ((m_vcMsg.size() != rfJarray.size()))
		{
			throw std::runtime_error("STD_Text::LoadViaJson Json Error!");
		}

		for (size_t ite = 0; ite < m_vcMsg.size(); ite++)
		{
			m_vcMsg[ite].Load(rfJarray[ite], nCodePage);
		}
	}


	void TextParser::WriteCodeBlock(Text_Code_Block& rfBlock, uint8_t* pAppend, size_t& nAppendWriteSize) const
	{
		// Check if there is enough space to write goto commnad
		(rfBlock.GetEndOffset() - rfBlock.GetBegOffset()) < (sizeof(uint16_t) + sizeof(uint32_t)) ? (throw std::runtime_error("Not enough space to write goto commnad")) : (void)(0);

		// write jmp [goto command]
		*(uint16_t*)(m_Sdt.GetCodePtr() + rfBlock.GetBegOffset()) = 0x0A42;
		*(uint32_t*)(m_Sdt.GetCodePtr() + rfBlock.GetBegOffset() + sizeof(uint16_t)) = m_Sdt.GetSdtSize() + nAppendWriteSize - sizeof(VAL_SDT_HDR_Info);

		// write jmp [goto command] target offset
		*(uint32_t*)(pAppend + nAppendWriteSize) = m_Sdt.GetCodeSize() + nAppendWriteSize + 4;
		nAppendWriteSize += sizeof(uint32_t);

		// write msg code struct
		memcpy(pAppend + nAppendWriteSize, rfBlock.GetPtr(), rfBlock.GetSize());
		nAppendWriteSize += rfBlock.GetSize();

		// write ret [goto command]
		*(uint16_t*)(pAppend + nAppendWriteSize) = 0x0A42;
		*(uint32_t*)(pAppend + nAppendWriteSize + sizeof(uint16_t)) = m_Sdt.GetSdtSize() + nAppendWriteSize + (sizeof(uint16_t) + sizeof(uint32_t)) - sizeof(VAL_SDT_HDR_Info);;
		nAppendWriteSize += (sizeof(uint16_t) + sizeof(uint32_t));

		// write ret [goto command] target offset
		*(uint32_t*)(pAppend + nAppendWriteSize) = rfBlock.GetEndOffset();
		nAppendWriteSize += sizeof(uint32_t);
	}

	Rut::RxMem::Auto TextParser::Make()
	{
		// Merge Adjacent Code
		std::vector<Text_Code_Block> code_mem_list;
		{
			size_t end_offset = m_vcMsg.front().GetBegOffset();
			std::vector<TextCode> code_block;
			for (auto ite = m_vcMsg.begin(); ite != m_vcMsg.end(); ite++)
			{
				TextCode& unit = *ite;
				if (end_offset == unit.GetBegOffset())
				{
					end_offset = unit.GetEndOffset();
					code_block.push_back(std::move(unit));
				}
				else
				{
					code_mem_list.emplace_back(code_block);
					code_block.clear();
					end_offset = unit.GetEndOffset();
					code_block.push_back(std::move(unit));
				}
			}

			if (code_block.size())
			{
				code_mem_list.emplace_back(code_block);
			}
		}

		// Create Append Buffer
		Rut::RxMem::Auto append_mem;
		{
			size_t append_mem_size = 0;

			for (auto& block : code_mem_list)
			{
				append_mem_size += 4 + block.GetSize() + 2 + 4 + 4;
			}

			append_mem.SetSize(append_mem_size);
		}

		// Write Code
		{
			size_t write_size = 0;
			for (auto& block : code_mem_list)
			{
				this->WriteCodeBlock(block, append_mem.GetPtr(), write_size);
			}
		}

		// sign sdt file
		{
			uint8_t* check_data_ptr = m_Sdt.GetSdtPtr() + m_Sdt.GetInfoPtr()->uiCheckDataFOA;
			size_t check_data_bytes = (size_t)(m_Sdt.GetInfoPtr()->uiHDRSize - m_Sdt.GetInfoPtr()->uiCheckDataFOA - 1);
			size_t sdt_org_size = m_Sdt.GetSdtSize();
			size_t sdt_new_size = m_Sdt.GetSdtSize() + append_mem.GetSize();
			SDT::Signer::Sign({ check_data_ptr, check_data_bytes }, sdt_org_size, sdt_new_size);
		}

		return { m_Sdt.GetMem(), append_mem};
	}

	Rut::RxJson::JArray TextParser::Make(size_t nCodePage) const
	{
		Rut::RxJson::JArray obj_list_json;

		for (auto& msg : m_vcMsg) 
		{ 
			obj_list_json.emplace_back(msg.Make(nCodePage));
		}

		return obj_list_json;
	}

	bool TextParser::ParseText()
	{
		if (this->GetSdtFile().GetMsgCount())
		{
			this->Scan();
			if (this->GetMsgCount())
			{
				return true;
			}
		}
		return false;
	}

	Rut::RxJson::JArray TextParser::ReadText(size_t nCodePage) const
	{
		Rut::RxJson::JArray text_list, index_list;
		Rut::RxJson::JArray obj_json_list = this->Make(nCodePage);
		for (auto ite : std::views::iota(0u, obj_json_list.size()))
		{
			Rut::RxJson::JValue& obj_json = obj_json_list[ite];
			std::wstring_view obj_name = obj_json[L"Name"];

			if (obj_name == L"MsgText" || obj_name == L"MsgName" || obj_name == L"SelectText" || obj_name == L"StrSet")
			{
				Rut::RxJson::JValue text_json_entry;
				text_json_entry[L"org"] = obj_json[L"Text"];
				text_json_entry[L"tra"] = obj_json[L"Text"];
				text_list.push_back(std::move(text_json_entry));
				index_list.push_back((int)ite);
			}
		}

		Rut::RxJson::JArray text_json;
		text_json.push_back(std::move(text_list));
		text_json.push_back(std::move(index_list));
		return text_json;
	}

	void TextParser::LoadText(const std::filesystem::path& phTextJson, size_t nCodePage)
	{
		Rut::RxJson::JValue txt_json = Rut::RxJson::Parser{}.Load(phTextJson);
		Rut::RxJson::JArray obj_json_list = this->Make(nCodePage);
		Rut::RxJson::JArray& txt_json_list = txt_json.ToAry();
		Rut::RxJson::JArray& text_list = txt_json_list[0];
		Rut::RxJson::JArray& index_list = txt_json_list[1];

		if (text_list.size() != index_list.size())
		{
			throw std::runtime_error("Merge Text Json Mismatching!");
		}

		size_t text_count = text_list.size();
		for (size_t ite = 0; ite < text_count; ite++)
		{
			size_t index = (size_t)index_list[ite].ToInt();
			auto& text_entry = text_list[ite];
			obj_json_list[index][L"Text"] = text_entry[L"tra"];
		}

		this->Load(obj_json_list, nCodePage);
	}

	const size_t TextParser::GetMsgCount() const noexcept
	{
		return m_vcMsg.size();
	}

	const SDT::File_Parser& TextParser::GetSdtFile() const noexcept
	{
		return m_Sdt;
	}


	Text_Test::Text_Test()
	{

	}

	void Text_Test::Parse(Rut::RxMem::Auto& amCode)
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
				SDT::Code::MsgName msg_name_code;
				msg_name_code.Load(code_ptr + ite_byte);
				{
					CheckMakeData(code_ptr + ite_byte, msg_name_code);
				}
				ite_byte += msg_name_code.GetSize();
				m_vcMsgNameCodes.push_back(std::move(msg_name_code));
			}
			else if (memcmp(code_ptr + ite_byte, search_msg_text_code, sizeof(search_msg_text_code)) == 0)
			{
				SDT::Code::MsgText msg_text_code;
				msg_text_code.Load(code_ptr + ite_byte);
				{
					CheckMakeData(code_ptr + ite_byte, msg_text_code);
				}
				ite_byte += msg_text_code.GetSize();
				m_vcMsgTextCodes.emplace_back(std::move(msg_text_code));

				if (*(uint16_t*)(code_ptr + ite_byte) == 0x0E04)
				{
					SDT::Code::MsgNewLine msg_newline_code;
					msg_newline_code.Load(code_ptr + ite_byte);
					{
						CheckMakeData(code_ptr + ite_byte, msg_newline_code);
					}
					ite_byte += msg_newline_code.GetSize();
					m_vcMsgNewLineCodes.push_back(std::move(msg_newline_code));
				}
			}
			else if (memcmp(code_ptr + ite_byte, search_select_text_code, sizeof(search_select_text_code)) == 0)
			{
				SDT::Code::SelectText select_text_code;
				select_text_code.Load(code_ptr + ite_byte);
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
}