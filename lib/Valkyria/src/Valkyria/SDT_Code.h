#pragma once
#include <vector>
#include <stdexcept>

#include <Rut/RxMem.h>
#include <Rut/RxJson.h>


namespace Valkyria::SDT::Code
{
	class MsgName
	{
	private:
		uint16_t m_usOP = 0;
		std::string m_msText;

	public:
		MsgName();
		MsgName(const uint8_t* const pData);

		void Load(const uint8_t* const pData);
		void Load(Rut::RxJson::JValue& rfJson, size_t nCodePage);
		void Make(Rut::RxMem::Auto& rfMem) const;
		Rut::RxJson::JValue Make(size_t nCodePage) const;

	public:
		constexpr uint16_t GetOP() const;
		constexpr size_t GetSize() const;
		std::wstring GetText(size_t nCodePage) const;
		void SetText(std::wstring_view wsText, size_t nCodePage);
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
		MsgText(const uint8_t* const pData);

		void Load(const uint8_t* const pData);
		void Load(Rut::RxJson::JValue& rfJson, size_t nCodePage);
		void Make(Rut::RxMem::Auto& rfMem) const;
		Rut::RxJson::JValue Make(size_t nCodePage) const;

	public:
		constexpr uint16_t GetOP() const;
		constexpr size_t GetSize() const;
		std::wstring GetText(size_t nCodePage) const;
		void SetText(std::wstring_view wsText, size_t nCodePage);
	};

	class MsgNewLine
	{
	private:
		uint16_t m_usOP = 0;
		uint16_t m_usUnknow = 0;
		uint32_t m_uiLineNumber = 0;

	public:
		MsgNewLine();
		MsgNewLine(const uint8_t* const pData);

		void Load(const uint8_t* const pData);
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
		SelectText(const uint8_t* const pData);

		void Load(const uint8_t* const pData);
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
		SetStr(const uint8_t* const pData);

		void Load(const uint8_t* const pData);
		void Load(Rut::RxJson::JValue& rfJson, size_t nCodePage);
		void Make(Rut::RxMem::Auto& rfMem) const;
		Rut::RxJson::JValue Make(size_t nCodePage) const;

	public:
		constexpr uint16_t GetOP() const;
		const size_t GetSize() const;
		std::wstring GetText(size_t nCodePage) const;
		void SetText(std::wstring_view wsText, size_t nCodePage);
	};
}