#pragma once
#include <vector>
#include <stdexcept>
#include <ZxMem/ZxMem.h>
#include <ZxJson/JValue.h>


namespace ZQF::RxValkyria::SDT::Code
{
	class MsgName
	{
	private:
		std::uint16_t m_usOP = 0;
		std::string m_msText;

	public:
		MsgName();
		MsgName(const std::uint8_t* const pData);

	public:
		auto Load(const std::uint8_t* const pData) -> void;
		auto Load(const ZxJson::JValue& rfJson, std::size_t nCodePage) -> void;
		auto Make(ZxMem& rfMem) const -> void;
		auto Make(std::size_t nCodePage) const -> ZxJson::JValue;

	public:
		auto GetOP() const -> std::uint16_t;
		auto GetText(std::size_t nCodePage) const -> std::string;

	public:
		auto SetText(std::string_view wsText, std::size_t nCodePage) -> void;

	public:
		auto SizeBytes() const -> std::size_t;
	};

	class MsgText
	{
	private:
		std::uint16_t m_usOP = 0;
		std::uint16_t m_usUnknow = 0;
		std::uint32_t m_uiLineNumber = 0;
		std::string m_msText;

	public:
		MsgText();
		MsgText(const std::uint8_t* const pData);

	public:
		auto Load(const std::uint8_t* const pData) -> void;
		auto Load(const ZxJson::JValue& rfJson, std::size_t nCodePage) -> void;
		auto Make(ZxMem& rfMem) const -> void;
		auto Make(std::size_t nCodePage) const -> ZxJson::JValue;

	public:
		auto GetOP() const -> std::uint16_t;
		auto GetText(std::size_t nCodePage) const -> std::string;

	public:
		auto SetText(std::string_view wsText, std::size_t nCodePage) -> void;

	public:
		auto SizeBytes() const -> std::size_t;
	};

	class MsgNewLine
	{
	private:
		std::uint16_t m_usOP = 0;
		std::uint16_t m_usUnknow = 0;
		std::uint32_t m_uiLineNumber = 0;

	public:
		MsgNewLine();
		MsgNewLine(const std::uint8_t* const pData);

	public:
		auto Load(const std::uint8_t* const pData) -> void;
		auto Load(const ZxJson::JValue& rfJson, std::size_t nCodePage) -> void;
		auto Make(ZxMem& rfMem) const -> void;
		auto Make(std::size_t nCodePage) const -> ZxJson::JValue;

	public:
		auto GetOP() const -> std::uint16_t;

	public:
		auto SizeBytes() const -> std::size_t;
	};

	class SelectText
	{
	private:
		std::uint16_t m_usOP = 0;
		std::uint32_t m_usUnknow = 0;
		std::vector<std::string> m_vcText;

	public:
		SelectText();
		SelectText(const std::uint8_t* const pData);

	public:
		auto Load(const std::uint8_t* const pData) -> void;
		auto Load(const ZxJson::JValue& rfJson, std::size_t nCodePage) -> void;
		auto Make(ZxMem& rfMem) const -> void;
		auto Make(std::size_t nCodePage) const -> ZxJson::JValue;

	public:
		auto GetOP() const->std::uint16_t;

	public:
		auto SizeBytes() const->std::size_t;
	};

	class SetStr
	{
	private:
		std::uint16_t m_usOP = 0;
		std::uint8_t m_ucStrType = 0;
		std::uint32_t m_uiSPIndex = 0;
		std::uint8_t m_ucStrDataType = 0;
		std::string m_msText;

	public:
		SetStr();
		SetStr(const std::uint8_t* const pData);

	public:
		auto Load(const std::uint8_t* const pData) -> void;
		auto Load(const ZxJson::JValue& rfJson, std::size_t nCodePage) -> void;
		auto Make(ZxMem& rfMem) const -> void;
		auto Make(std::size_t nCodePage) const -> ZxJson::JValue;

	public:
		auto GetOP() const -> std::uint16_t;
		auto GetText(std::size_t nCodePage) const -> std::string;

	public:
		auto SetText(std::string_view wsText, std::size_t nCodePage) -> void;

	public:
		auto SizeBytes() const->std::size_t;
	};
}