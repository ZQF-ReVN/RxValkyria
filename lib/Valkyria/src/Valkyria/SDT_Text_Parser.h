#pragma once
#include <algorithm>

#include "SDT_File.h"
#include "SDT_Text_Code.h"
#include "Valkyria_Types.h"


namespace Valkyria::SDT
{
	class Text_Parser
	{
	private:
		SDT::File_Parser m_Sdt;
		std::vector<Text_Code> m_vcMsg;

	private:

		void WriteCodeBlock(Text_Code_Block& rfBlock, uint8_t* pAppend, size_t& nAppendWriteSize) const;

	public:
		Text_Parser();
		Text_Parser(std::wstring_view wsPath);

		void Scan();
		void Read(std::wstring_view wsPath);
		void Load(Rut::RxJson::JValue& rfJarray, size_t nCodePage);
		Rut::RxMem::Auto Make();
		Rut::RxJson::JValue Make(size_t nCodePage) const;

	public:
		const size_t GetMsgCount() const noexcept;
		const SDT::File_Parser& GetSdtFile() const noexcept;
	};

	template<typename SDT_OBJ_T>
	static void CheckMakeData(uint8_t* pOrg, SDT_OBJ_T& rfOBJ)
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
