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
		uint16_t m_usOPCode = 0;
		std::string m_msText;

	public:
		MsgName();
		MsgName(uint8_t* pData);

		void Parse(uint8_t* pData);
		Rut::RxMem::Auto Make() const;

	public:
		void SetText(std::string msText);
		const std::string& GetText() const;
		constexpr uint16_t GetOPCode() const;
		constexpr size_t GetSize() const;
	};

	class MsgText
	{
	private:
		uint16_t m_usOPCode = 0;
		uint16_t m_usUnValue = 0;
		uint32_t m_uiLineNumer = 0;
		std::string m_msText;

	public:
		MsgText();
		MsgText(uint8_t* pData);

		void Parse(uint8_t* pData);
		Rut::RxMem::Auto Make() const;

	public:
		void SetText(std::string msText);
		const std::string& GetText() const;
		constexpr uint16_t GetOPCode() const;
		constexpr size_t GetSize() const;
	};

	class MsgNewLine
	{
	private:
		uint16_t m_usOPCode = 0;
		uint16_t m_usUnValue = 0;
		uint32_t m_uiLineNumer = 0;

	public:
		MsgNewLine();
		MsgNewLine(uint8_t* pData);

		void Parse(uint8_t* pData);
		Rut::RxMem::Auto Make() const;

	public:
		constexpr uint16_t GetOPCode() const;
		constexpr size_t GetSize() const;
	};

	class SelectText
	{
	private:
		uint16_t m_usOPCode = 0;
		uint32_t m_usUnValue = 0;
		std::vector<std::string> m_vcText;

	public:
		SelectText();
		SelectText(uint8_t* pData);

		void Parse(uint8_t* pData);
		Rut::RxMem::Auto Make() const;

	public:
		void SetTexts(std::vector<std::string>& vcText);
		const std::vector<std::string>& GetTexts() const;
		constexpr uint16_t GetOPCode() const;
		constexpr size_t GetSize() const;
	};
}