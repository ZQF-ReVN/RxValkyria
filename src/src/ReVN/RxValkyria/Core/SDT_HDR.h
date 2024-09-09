#pragma once
#include <vector>
#include <stdexcept>
#include <Zut/ZxMem.h>


namespace ZQF::ReVN::RxValkyria::SDT
{
	class HDR_Info
	{
	private:
		std::uint32_t m_uiHeaderSize{};
		std::uint32_t m_uiMsgCount{};
		std::uint32_t m_uiSelectCount{};
		std::uint32_t m_uiLabelCount{};
		std::uint32_t m_uiCheckDataFOA{};

	public:
		HDR_Info();

	public:
		auto Load(const std::uint8_t* const pData) -> void;
		auto Make(ZxMem& rfMem) const -> void;

	public:
		auto SizeBytes() const noexcept -> std::size_t;

	public:
		auto GetHeaderSize() const noexcept -> std::size_t;
		auto GetLabelCount() const noexcept -> std::size_t;
		auto GetMsgCount() const noexcept -> std::size_t;
		auto GetSelectCount() const noexcept -> std::size_t;
		auto GetCheckDataSize() const noexcept -> std::size_t;
	};

	class Label_Entry
	{
	private:
		std::string m_msLabelName;
		std::uint32_t m_uiLableInCodeOffset{};

	public:
		Label_Entry();

	public:
		auto Load(const std::uint8_t* const pData) -> void;
		auto Make(ZxMem& rfMem) const -> void;

	public:
		auto SizeBytes() const -> std::size_t;
	};

	class Label_Index
	{
	private:
		std::vector<Label_Entry> m_vcLabels;

	public:
		Label_Index();

		auto Load(const std::uint8_t* const pData, const std::size_t uiLabelCount) -> void;
		auto Make(ZxMem& rfMem) const -> void;

	public:
		auto SizeBytes() const -> std::size_t;
	};

	class HDR
	{
	private:
		HDR_Info m_Info;
		Label_Index m_Labels;
		std::string m_msCheckData;

	public:
		HDR();

	public:
		auto Load(const std::uint8_t* const pData) -> void;
		auto Make(ZxMem& rfMem) const -> void;

	public:
		auto SizeBytes() const -> std::size_t;

	public:
		auto GetInfo() const -> const HDR_Info&;
		auto GetCheckDataSize() const -> std::size_t;
		auto SetCheckData(const std::string_view msCheckData) -> void;
	};
}