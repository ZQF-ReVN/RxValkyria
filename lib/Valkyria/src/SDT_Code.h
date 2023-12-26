#pragma once
#include <vector>
#include <stdexcept>

#include "../../Rut/RxMem.h"
#include "../../Rut/RxJson.h"


namespace Valkyria::SDT::Code
{
	class MsgName
	{
	private:
		uint16_t m_usOP = 0;
		std::string m_msText;

	public:
		MsgName();
		MsgName(uint8_t* const pData);

		void Load(uint8_t* const pData);
		void Load(Rut::RxJson::JValue& rfJson, size_t nCodePage);
		void Make(Rut::RxMem::Auto& rfMem) const;
		Rut::RxJson::JValue Make(size_t nCodePage) const;

	public:
		constexpr uint16_t GetOP() const;
		constexpr size_t GetSize() const;
	};

	class MsgText
	{
	private:
		uint16_t m_usOP = 0;
		uint16_t m_usUnknow = 0;
		uint32_t m_uiLineNumber = 0;
		std::string m_msText;

	public:
		MsgText();
		MsgText(uint8_t* const pData);

		void Load(uint8_t* const pData);
		void Load(Rut::RxJson::JValue& rfJson, size_t nCodePage);
		void Make(Rut::RxMem::Auto& rfMem) const;
		Rut::RxJson::JValue Make(size_t nCodePage) const;

	public:
		constexpr uint16_t GetOP() const;
		constexpr size_t GetSize() const;
	};

	class MsgNewLine
	{
	private:
		uint16_t m_usOP = 0;
		uint16_t m_usUnknow = 0;
		uint32_t m_uiLineNumber = 0;

	public:
		MsgNewLine();
		MsgNewLine(uint8_t* const pData);

		void Load(uint8_t* const pData);
		void Load(Rut::RxJson::JValue& rfJson, size_t nCodePage);
		void Make(Rut::RxMem::Auto& rfMem) const;
		Rut::RxJson::JValue Make(size_t nCodePage) const;

	public:
		constexpr uint16_t GetOP() const;
		constexpr size_t GetSize() const;
	};

	class SelectText
	{
	private:
		uint16_t m_usOP = 0;
		uint32_t m_usUnknow = 0;
		std::vector<std::string> m_vcText;

	public:
		SelectText();
		SelectText(uint8_t* const pData);

		void Load(uint8_t* const pData);
		void Load(Rut::RxJson::JValue& rfJson, size_t nCodePage);
		void Make(Rut::RxMem::Auto& rfMem) const;
		Rut::RxJson::JValue Make(size_t nCodePage) const;

	public:
		constexpr uint16_t GetOP() const;
		constexpr size_t GetSize() const;
	};

	class SetStr
	{
	private:
		uint16_t m_usOP = 0;
		uint8_t m_ucStrType = 0;
		uint32_t m_uiUnknow = 0;
		uint8_t m_ucStrDataType = 0;
		std::string m_msText;

	public:
		SetStr();
		SetStr(uint8_t* const pData);

		void Load(uint8_t* const pData);
		void Load(Rut::RxJson::JValue& rfJson, size_t nCodePage);
		void Make(Rut::RxMem::Auto& rfMem) const;
		Rut::RxJson::JValue Make(size_t nCodePage) const;

	public:
		constexpr uint16_t GetOP() const;
		const size_t GetSize() const;
	};
}