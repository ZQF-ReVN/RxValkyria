#include "SDT_View.h"


namespace ZQF::ReVN::RxValkyria::SDT
{
	View::View()
	{

	}

	View::View(const std::span<std::uint8_t> spSdt)
		:m_spSdt{ spSdt }
	{

	}

	auto View::Assign(const std::span<std::uint8_t> spSdt) -> void
	{
		m_spSdt = spSdt;
	}

	auto View::GetHDRInfo() const noexcept -> Struct::SDT_HDR_Info&
	{
		return *reinterpret_cast<Struct::SDT_HDR_Info*>(this->GetPtr());
	}

	auto View::GetMsgCnt() const noexcept -> std::size_t
	{
		return this->GetHDRInfo().uiMsgCount;
	}

	auto View::GetCodePtr() const noexcept -> std::uint8_t*
	{
		return this->GetPtr() + this->GetHDRInfo().uiHDRSize;
	}

	auto View::GetCodeBytes() const noexcept -> std::size_t
	{
		return this->GetBytes() - this->GetHDRInfo().uiHDRSize;
	}

	auto View::GetBytes() const noexcept -> std::size_t
	{
		return m_spSdt.size_bytes();
	}

	auto View::GetPtr() const noexcept -> std::uint8_t*
	{
		return m_spSdt.data();
	}
}