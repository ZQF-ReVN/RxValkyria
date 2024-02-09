#pragma once
#include <ranges>
#include <filesystem>
#include <algorithm>

#include "SDT_File.h"
#include "SDT_TextCode.h"
#include "Valkyria_Types.h"


namespace Valkyria::SDT
{
	class TextParser
	{
	private:
		SDT::File_Parser m_Sdt;
		std::vector<TextCode> m_vcMsg;

	private:
		void WriteCodeBlock(Text_Code_Block& rfBlock, uint8_t* pAppend, size_t& nAppendWriteSize) const;

	public:
		TextParser();
		TextParser(const std::filesystem::path& phSdt);

	public:
		void Scan();
		void Read(const std::filesystem::path& phSdt);
		void Load(Rut::RxJson::JArray& rfJarray, size_t nCodePage);
		Rut::RxMem::Auto Make();
		Rut::RxJson::JArray Make(size_t nCodePage) const;

	public:
		bool ParseText();
		Rut::RxJson::JArray ReadText(size_t nCodePage) const;
		void LoadText(const std::filesystem::path& phTextJson, size_t nCodePage);

	public:
		const size_t GetMsgCount() const noexcept;
		const SDT::File_Parser& GetSdtFile() const noexcept;
	};

	template<typename T>
	static void CheckMakeData(uint8_t* pOrg, T& rfOBJ)
	{
		Rut::RxMem::Auto mem;
		rfOBJ.Make(mem);
		if (memcmp(pOrg, mem.GetPtr(), mem.GetSize()))
		{
			throw std::runtime_error("Dump Data Mismatched!");
		}
	}

	class Text_Test
	{
	private:
		std::vector<SDT::Code::MsgName> m_vcMsgNameCodes;
		std::vector<SDT::Code::MsgText> m_vcMsgTextCodes;
		std::vector<SDT::Code::MsgNewLine> m_vcMsgNewLineCodes;
		std::vector<SDT::Code::SelectText> m_vcSelectTextCode;

	public:
		Text_Test();
		void Parse(Rut::RxMem::Auto& amCode);
	};
}
