﻿#include <RxValkyria/Core/SDT_TextParser.h>
#include <RxValkyria/Core/SDT_String.h>
#include <RxValkyria/Core/SDT_Signer.h>
#include <Zut/ZxJson.h>
#include <ranges>


namespace ZQF::RxValkyria::SDT
{
	TextParser::TextParser()
	{

	}

	TextParser::TextParser(const std::string_view msSdtPath)
	{
		this->LoadViaFile(msSdtPath);
		this->Scan();
	}

	auto TextParser::Scan() -> void
	{
		auto fn_check_target_code = [](const std::uint8_t* pMem) -> bool
			{
				constexpr std::uint8_t const msg_text_code_search_mask[] = { 0x01, 0x0E, 0x11, 0x11 }; // msg
				constexpr std::uint8_t const new_line_code_search_mask[] = { 0x04, 0x0E ,0x11, 0x11 }; // new line
				constexpr std::uint8_t const msg_name_code_search_mask[] = { 0x00, 0x0E, 0x7E, 0x86 }; // msg name
				constexpr std::uint8_t const select_code_search_mask[] = { 0x1C, 0x0E, 0x00, 0x00, 0x00, 0x00 }; // select 
				constexpr std::uint8_t const set_str_code_search_mask[] = { 0x17, 0x0B, 0x09, 0x00, 0x00, 0x00, 0x00, 0x08 }; // setStr 

				if ((std::memcmp(pMem, msg_text_code_search_mask, sizeof(msg_text_code_search_mask)) == 0) ||
					(std::memcmp(pMem, new_line_code_search_mask, sizeof(new_line_code_search_mask)) == 0) ||
					(std::memcmp(pMem, msg_name_code_search_mask, sizeof(msg_name_code_search_mask)) == 0) ||
					(std::memcmp(pMem, select_code_search_mask, sizeof(select_code_search_mask)) == 0) ||
					(std::memcmp(pMem, set_str_code_search_mask, sizeof(set_str_code_search_mask)) == 0))
				{
					return true;
				}
				else
				{
					return false;
				}
			};

		if (m_vcMsg.size()) { m_vcMsg.clear(); }
		if (m_mSdt.SizeBytes() < 0x10) { return; }

		SDT::View sdt_view{ m_mSdt.Span() };

		const std::uint8_t* code_ptr = sdt_view.GetCodePtr();
		const std::size_t max_search_size = sdt_view.GetCodeBytes() - 4;

		for (std::size_t ite_byte = 0; ite_byte < max_search_size;)
		{
			const std::uint8_t* cur_ptr = code_ptr + ite_byte;
			if (fn_check_target_code(cur_ptr))
			{
				SDT::TextCode msg{ code_ptr, ite_byte };
				ite_byte += msg.SizeBytes();
				m_vcMsg.push_back(std::move(msg));
				continue;
			}
			ite_byte++;
		}
	}

	auto TextParser::LoadViaFile(const std::string_view msSdtPath) -> void
	{
		m_mSdt.Load(msSdtPath);
	}

	auto TextParser::LoadViaJson(const ZxJson::JArray_t& rfMsgJson, const std::size_t nCodePage) -> void
	{
		if (m_vcMsg.size() != rfMsgJson.size())
		{
			throw std::runtime_error("RxValkyria::SDT::TextParser::Load(): json error!");
		}

		for (const auto&[msg, json] : std::views::zip(m_vcMsg, rfMsgJson))
		{
			msg.Load(json, nCodePage);
		}
	}

	auto TextParser::MakeBin() const -> ZxMem
	{
		// merge adjacent code
		std::size_t append_mem_bytes{};
		std::vector<TextCodeBlock> text_code_block_vec;
		{
			std::size_t end_offset{ m_vcMsg.front().GetBegOffset() };
			std::vector<TextCode> code_block;
			for (const auto& unit : m_vcMsg)
			{
				if (end_offset == unit.GetBegOffset())
				{
					end_offset = unit.GetEndOffset();
					code_block.push_back(unit);
				}
				else
				{
					text_code_block_vec.emplace_back(code_block);
					append_mem_bytes += 4 + text_code_block_vec.back().SizeBytes() + 2 + 4 + 4;

					code_block.clear();
					end_offset = unit.GetEndOffset();
					code_block.push_back(unit);
				}
			}

			if (code_block.size())
			{
				text_code_block_vec.emplace_back(code_block);
				append_mem_bytes += 4 + text_code_block_vec.back().SizeBytes() + 2 + 4 + 4;
			}
		}

		// alloc new sdt file data mem
		const auto sdt_org_size = m_mSdt.SizeBytes();
		const auto sdt_new_size = sdt_org_size + append_mem_bytes;
		ZxMem sdt_mem{ sdt_new_size };

		// write org sdt file data
		sdt_mem << m_mSdt.Span();

		// write code to append buffer
		ZxMem append_mem{ append_mem_bytes };
		{
			SDT::View sdt_view{ std::span{ sdt_mem.Ptr(), sdt_mem.PosCur() } };
			const auto sdt_code_ptr = sdt_view.GetCodePtr();
			const auto sdt_code_bytes = sdt_view.GetCodeBytes();
			constexpr auto goto_cmd_bytes = sizeof(std::uint16_t) + sizeof(std::uint32_t);
			for (auto& code_block : text_code_block_vec)
			{
				// OP: 0x0A42
				// Name : GOTO
				// Formate : [word:op] [dword:label_index_table_offset]
				// Example : 420A 61000000
				
				// Check if there is enough space to write goto commnad
				if ((code_block.GetEndOffset() - code_block.GetBegOffset()) < goto_cmd_bytes)
				{
					throw std::runtime_error("RxValkyria::SDT::TextParser::MakeBin(): Not enough space to write goto commnad");
				}

				const auto cur_sdt_code_ptr = sdt_code_ptr + code_block.GetBegOffset();

				// write jmp GOTO command
				*reinterpret_cast<std::uint16_t*>(cur_sdt_code_ptr) = 0x0A42;
				// write jmp GOTO command (target offset) offset
				*reinterpret_cast<std::uint32_t*>(cur_sdt_code_ptr + sizeof(std::uint16_t)) = static_cast<std::uint32_t>(sdt_org_size - sizeof(VAL_SDT_HDR_Info) + append_mem.PosCur());
				// write jmp GOTO command target offset
				append_mem << static_cast<std::uint32_t>(sdt_code_bytes + append_mem.PosCur() + 4);

				// write msg code struct
				append_mem << code_block.GetMem().Span();

				// write ret GOTO command
				append_mem << static_cast<std::uint16_t>(0x0A42);
				// write ret GOTO command (target offset) offset
				append_mem << static_cast<std::uint32_t>(sdt_org_size - sizeof(VAL_SDT_HDR_Info) + (append_mem.PosCur() - sizeof(std::uint16_t)) + goto_cmd_bytes);
				// write ret GOTO command target offset
				append_mem << static_cast<std::uint32_t>(code_block.GetEndOffset());
			}
		}

		// write append data
		sdt_mem << append_mem.Span();

		// sign sdt file
		{
			const auto hdr_info_ptr = sdt_mem.Ptr<VAL_SDT_HDR_Info*>();
			const auto check_data_ptr = sdt_mem.Ptr() + hdr_info_ptr->uiCheckDataFOA;
			const auto check_data_bytes = static_cast<std::size_t>(hdr_info_ptr->uiHDRSize - hdr_info_ptr->uiCheckDataFOA - 1);
			SDT::Signer::Sign({ check_data_ptr, check_data_bytes }, sdt_org_size, sdt_new_size);
		}

		return sdt_mem;
	}

	auto TextParser::MakeJson(const std::size_t nCodePage) const -> ZxJson::JArray_t
	{
		ZxJson::JArray_t obj_list_json;
		for (auto& msg : m_vcMsg)
		{
			obj_list_json.emplace_back(msg.Make(nCodePage));
		}
		return obj_list_json;
	}

	auto TextParser::GetMsgCnt() const noexcept -> std::size_t
	{
		return m_vcMsg.size();
	}

	auto TextParser::GetSdtFile() const noexcept -> const ZxMem&
	{
		return m_mSdt;
	}


	TextParserJsonManager::TextParserJsonManager(const std::string_view msSdtPath)
		:TextParser(msSdtPath)
	{

	}

	auto TextParserJsonManager::Parse() -> bool
	{
		this->TextParser::Scan();
		return this->TextParser::GetMsgCnt() ? true : false;
	}

	auto TextParserJsonManager::Export(const std::size_t nCodePage) const -> ZxJson::JArray_t
	{
		ZxJson::JArray_t text_list, index_list;
		const auto obj_json_vector = this->TextParser::MakeJson(nCodePage);
		for (const auto [idx, obj_json] : std::views::enumerate(obj_json_vector))
		{
			const auto obj_name = obj_json.At("Name").GetStrView();
			if (!(obj_name == "MsgText" || obj_name == "MsgName" || obj_name == "SelectText" || obj_name == "StrSet")) { continue; }

			const auto& text_json = obj_json.At("Text");
			text_list.emplace_back(ZxJson::JObject_t{ { "org", text_json }, { "tra", text_json } });
			index_list.emplace_back(static_cast<std::size_t>(idx));
		}

		return { std::move(text_list), std::move(index_list) };
	}

	auto TextParserJsonManager::Export(const std::string_view msJsonPath, const std::size_t nCodePage) const -> void
	{
		ZxJson::JValue text_json{ this->Export(nCodePage) };
		ZxJson::StoreViaFile(msJsonPath, text_json, true, true);
	}

	auto TextParserJsonManager::Import(const std::string_view msJsonPath, const std::size_t nCodePage) -> void
	{
		const auto msg_json = ZxJson::LoadViaFile(msJsonPath);
		const auto& msg_text_vec = msg_json[0].GetArray();
		const auto& msg_obj_idx_vec = msg_json[1].GetArray();
		if (msg_text_vec.size() != msg_obj_idx_vec.size())
		{
			throw std::runtime_error("RxValkyria::SDT::TextParser::LoadText(): text Json mismatching!");
		}

		auto obj_json_vec = this->TextParser::MakeJson(nCodePage);

		const auto text_count = msg_text_vec.size();
		for (std::size_t msg_idx{}; msg_idx < text_count; msg_idx++)
		{
			const auto obj_idx = msg_obj_idx_vec[msg_idx].GetNum<std::size_t>();
			obj_json_vec[obj_idx].At("Text") = msg_text_vec[msg_idx].At("tra");
		}

		this->TextParser::LoadViaJson(obj_json_vec, nCodePage);
	}

	auto TextParserJsonManager::Import(const std::string_view msSdtPath, const std::string_view msJsonPath, const std::size_t nCodePage) -> void
	{
		this->Import(msJsonPath, nCodePage);
		this->TextParser::MakeBin().Save(msSdtPath);
	}
}