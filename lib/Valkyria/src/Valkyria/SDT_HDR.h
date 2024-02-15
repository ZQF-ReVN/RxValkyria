#pragma once
#include <vector>
#include <stdexcept>

#include <Rut/RxMem.h>


namespace Valkyria::SDT
{
	class HDR_Info
	{
	private:
		uint32_t m_uiHeaderSize = 0;
		uint32_t m_uiMsgCount = 0;
		uint32_t m_uiSelectCount = 0;
		uint32_t m_uiLabelCount = 0;
		uint32_t m_uiCheckDataFOA = 0;

	public:
		HDR_Info();

		void Load(const uint8_t* const pData);
		void Make(Rut::RxMem::Auto& rfMem) const;

	public:
		constexpr size_t GetSize() const noexcept;
		constexpr size_t GetHeaderSize() const noexcept;
		constexpr size_t GetLabelCount() const noexcept;
		constexpr size_t GetMsgCount() const noexcept;
		constexpr size_t GetSelectCount() const noexcept;
		constexpr size_t GetCheckDataSize() const noexcept;
	};

	class Label_Entry
	{
	private:
		std::string m_msLabelName;
		uint32_t m_uiLableInCodeOffset = 0;

	public:
		Label_Entry();

		void Load(const uint8_t* const pData);
		void Make(Rut::RxMem::Auto& rfMem) const;

	public:
		constexpr size_t GetSize() const;
	};

	class Label_Index
	{
	private:
		std::vector<Label_Entry> m_vcLabels;

	public:
		Label_Index();

		void Load(const uint8_t* const pData, size_t uiLabelCount);
		void Make(Rut::RxMem::Auto& rfMem) const;

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

		void Load(const uint8_t* const pData);
		void Make(Rut::RxMem::Auto& rfMem) const;

	public:
		constexpr size_t GetSize() const;
		const HDR_Info& GetInfo() const;
		const size_t GetCheckDataSize() const;
		void SetCheckData(std::string_view msCheckData);
	};
}