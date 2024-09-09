#pragma once
#include <string_view>
#include <Zut/ZxMem.h>
#include <ReVN/RxValkyria/Core/Valkyria_Struct.h>


namespace ZQF::ReVN::RxValkyria::SDT
{
	class View
	{
	private:
		std::span<std::uint8_t> m_spSdt;

	public:
		View();
		View(const std::span<std::uint8_t> spSdt);

	public:
		auto Assign(const std::span<std::uint8_t> spSdt) -> void;

	public:
		auto GetHDRInfo() const noexcept -> Struct::SDT_HDR_Info&;
		auto GetMsgCnt() const noexcept -> std::size_t;
		auto GetCodePtr() const noexcept -> std::uint8_t*;
		auto GetCodeBytes() const noexcept -> std::size_t;

	public:
		auto GetPtr() const noexcept -> std::uint8_t*;
		auto GetBytes() const noexcept -> std::size_t;
	};
}