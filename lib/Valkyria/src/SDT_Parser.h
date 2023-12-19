#pragma once
#include <vector>
#include <stdexcept>

#include "../../Rut/RxMem.h"
#include "../../Rut/RxJson.h"


namespace Valkyria::SDT
{
	template<typename SDT_OBJ_T>
	static void CheckMakeData(uint8_t* pOrg, SDT_OBJ_T& rfOBJ)
	{
		Rut::RxMem::Auto mem = rfOBJ.Make();
		if (memcmp(pOrg, mem.GetPtr(), mem.GetSize()))
		{
			throw std::runtime_error("Dump Data Mismatched!");
		}
	}


	class HDR_Info
	{
	private:
		uint32_t m_uiHeaderSize = 0;
		uint32_t m_uiMsgCount = 0;
		uint32_t m_uiSelectCount = 0;
		uint32_t m_uiLabelCount = 0;
		uint32_t m_uiCheckDataRva = 0;

	public:
		HDR_Info();

		void Parse(uint8_t* pData);
		Rut::RxMem::Auto Make() const;

	public:
		constexpr size_t GetSize() const;
		constexpr size_t GetHeaderSize() const;
		constexpr size_t GetLabelCount() const;
		constexpr size_t GetMsgCount() const;
		constexpr size_t GetSelectCount() const;
	};

	class Label_Entry
	{
	private:
		std::string m_msLabelName;
		uint32_t m_uiLableInCodeOffset = 0;

	public:
		Label_Entry();

		void Parse(uint8_t* pData);
		Rut::RxMem::Auto Make() const;

	public:
		constexpr size_t GetSize() const;
	};

	class Label_Index
	{
	private:
		std::vector<Label_Entry> m_vcLabels;

	public:
		Label_Index();

		void Parse(uint8_t* pData, size_t uiLabelCount);
		Rut::RxMem::Auto Make() const;

	public:
		constexpr size_t GetSize() const;
	};

	class HDR
	{
	private:
		HDR_Info m_Info;
		Label_Index m_Labels;
		std::string m_msCheckData;

	public:
		HDR();

		void Parse(uint8_t* pData);
		Rut::RxMem::Auto Make() const;

	public:
		constexpr size_t GetSize() const;
		const HDR_Info& GetInfo() const;
		const size_t GetCheckDataSize() const;
		void SetCheckData(std::string_view msCheckData);
	};

	class Code_MsgName
	{
	private:
		uint16_t m_usOPCode = 0;
		std::string m_msText;

	public:
		Code_MsgName();
		Code_MsgName(uint8_t* pData);

		void Parse(uint8_t* pData);
		Rut::RxMem::Auto Make() const;

	public:
		void SetText(std::string msText);
		const std::string& GetText() const;
		constexpr uint16_t GetOPCode() const;
		constexpr size_t GetSize() const;
	};

	class Code_MsgText
	{
	private:
		uint16_t m_usOPCode = 0;
		uint16_t m_usUnValue = 0;
		uint32_t m_uiLineNumer = 0;
		std::string m_msText;

	public:
		Code_MsgText();
		Code_MsgText(uint8_t* pData);

		void Parse(uint8_t* pData);
		Rut::RxMem::Auto Make() const;

	public:
		void SetText(std::string msText);
		const std::string& GetText() const;
		constexpr uint16_t GetOPCode() const;
		constexpr size_t GetSize() const;
	};

	class Code_MsgNewLine
	{
	private:
		uint16_t m_usOPCode = 0;
		uint16_t m_usUnValue = 0;
		uint32_t m_uiLineNumer = 0;

	public:
		Code_MsgNewLine();
		Code_MsgNewLine(uint8_t* pData);

		void Parse(uint8_t* pData);
		Rut::RxMem::Auto Make() const;

	public:
		constexpr uint16_t GetOPCode() const;
		constexpr size_t GetSize() const;
	};

	class Code_SelectText
	{
	private:
		uint16_t m_usOPCode = 0;
		uint32_t m_usUnValue = 0;
		std::vector<std::string> m_vcText;

	public:
		Code_SelectText();
		Code_SelectText(uint8_t* pData);

		void Parse(uint8_t* pData);
		Rut::RxMem::Auto Make() const;

	public:
		void SetTexts(std::vector<std::string>& vcText);
		const std::vector<std::string>& GetTexts() const;
		constexpr uint16_t GetOPCode() const;
		constexpr size_t GetSize() const;
	};


	class File_Parser
	{
	private:
		HDR m_HDR;
		Rut::RxMem::Auto m_amCode;

	public:
		File_Parser();
		File_Parser(std::wstring_view wsPath);

		void Parse(std::wstring_view wsPath);
		void Parse(Rut::RxMem::Auto& amSDT);
		Rut::RxMem::Auto Make() const;

	public:
		Rut::RxMem::Auto& GetCodeMem();
		const HDR& GetHDR();
	};


	class Text_Test
	{
	private:
		std::vector<Code_MsgName> m_vcMsgNameCodes;
		std::vector<Code_MsgText> m_vcMsgTextCodes;
		std::vector<Code_MsgNewLine> m_vcMsgNewLineCodes;
		std::vector<Code_SelectText> m_vcSelectTextCode;

	public:
		Text_Test();
		void Parse(Rut::RxMem::Auto& amCode);
	};
}