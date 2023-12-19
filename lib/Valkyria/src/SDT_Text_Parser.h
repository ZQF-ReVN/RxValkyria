#pragma once
#include <algorithm>

#include "SDT_Parser.h"
#include "Valkyria_Types.h"


namespace Valkyria::SDT
{	 
	class Custom_Msg
	{
	private:
		Code_MsgName m_Name;
		Code_MsgText m_Text;
		Code_MsgNewLine m_NewLine;
		Code_SelectText m_Select;

		size_t m_uiCodeBegOffset = 0;
		size_t m_uiCodeEndOffset = 0;

	public:
		Custom_Msg();
		Custom_Msg(uint8_t* pCodeSeg, size_t uiCodeOffset);

		void Parse(uint8_t* pCodeSeg, size_t uiCodeOffset);
		Rut::RxMem::Auto Make() const;
		Rut::RxJson::JValue ToJson(size_t nCodePage, bool isDebug, size_t uiHdrSize) const;
		void Load(Rut::RxJson::JValue& rfJson, size_t nCodePage);

	public:
		constexpr size_t GetSize() const;
		constexpr size_t GetCodeBegOffset() const;
		constexpr size_t GetCodeEndOffset() const;
	};

	class Text_Parser
	{
	private:
		VAL_SDT_HDR_Info* m_pInfo = nullptr;
		Rut::RxMem::Auto m_amSDT;
		std::vector<Custom_Msg> m_vcMsg;

	public:
		Text_Parser();
		Text_Parser(std::wstring_view wsPath);
		void Init(std::wstring_view wsPath);

		void Parse();
		void Load(Rut::RxJson::JArray& rfJarray, size_t nCodePage);
		Rut::RxMem::Auto Make();
		Rut::RxJson::JValue ToJson(size_t nCodePage, bool isDebug) const;

	public:
		size_t GetMsgCountViaScan() const;
		size_t GetMsgCountViaInfo() const;
	};
}
