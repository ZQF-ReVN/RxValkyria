#include <RxValkyria/Core/SDT_Signer.h>
#include <RxValkyria/Core/Valkyria_Types.h>
#include <ZxMem/ZxMem.h>
#include <stdexcept>


namespace ZQF::RxValkyria::SDT
{
	Signer::Signer()
	{

	}

	Signer::Signer(const std::string_view msKey, const std::size_t nGameTitleLen)
		:m_msKey{ msKey }, m_uiGameTitleLen{ nGameTitleLen }
	{

	}

	// VAL-0064 VAL-0065 VAL-0066 VAL-0017Re
	auto Signer::InitViaSysData(const std::string_view msSysDataPath) -> void
	{
		ZxMem sys_data{ msSysDataPath };
		auto seg_0_ptr = sys_data.Ptr<VAL_SysData_Seg_0*>();
		m_uiGameTitleLen = std::strlen(seg_0_ptr->aGameTitle);
		m_msKey = this->DecodeKey({ reinterpret_cast<std::uint8_t*>(seg_0_ptr->aCheckKey), std::strlen(seg_0_ptr->aCheckKey) }, m_uiGameTitleLen);
	}

	auto Signer::EncodeKey() const -> ZxMem
	{
		ZxMem enc_key_mem(m_msKey.size());
		enc_key_mem << std::span{ m_msKey.data(), m_msKey.size() };
		Signer::SwapDataByteOrder(enc_key_mem.Span());
		Signer::EncodeBothEnds(enc_key_mem.Span(), m_uiGameTitleLen);
		return enc_key_mem;
	}

	auto Signer::MakeCheckData(const std::size_t nScriptSize) const -> ZxMem
	{
		if (m_msKey.empty()) { throw std::runtime_error("SDT::Signer: Error Not Key Data!"); }

		ZxMem check_data_mem{ this->EncodeKey() };

		const auto check_data_ptr{ check_data_mem.Ptr<uint8_t*>() };
		const auto check_data_size{ check_data_mem.SizeBytes() };

		for (std::size_t idx_byte{}; idx_byte < check_data_size; idx_byte++)
		{
			check_data_ptr[idx_byte] += static_cast<std::uint8_t>(nScriptSize);
		}

		return check_data_mem;
	}

	auto Signer::DecodeKey(const std::span<std::uint8_t> spKeyData, const std::size_t nGameTitleLen) -> std::string
	{
		ZxMem dec_key_mem{ spKeyData.size_bytes() };
		dec_key_mem << spKeyData;
		Signer::DecodeBothEnds(dec_key_mem.Span(), nGameTitleLen);
		Signer::SwapDataByteOrder(dec_key_mem.Span());
		return { dec_key_mem.Ptr<char*>(), dec_key_mem.SizeBytes() };
	}

	auto Signer::Sign(ZxMem& amSDT) const -> void
	{
		ZxMem check_mem{ this->MakeCheckData(amSDT.SizeBytes()) };

		const auto info_ptr = amSDT.PtrCur<VAL_SDT_HDR_Info*>();
		const auto check_data_size = static_cast<std::size_t>(info_ptr->uiHDRSize - info_ptr->uiCheckDataFOA);
		if (check_data_size != check_mem.SizeBytes()) { throw std::runtime_error("Signer::Sign: check data size mismatched!"); }

		amSDT
			.PosSet(info_ptr->uiCheckDataFOA)
			.Put(check_mem.Span())
			.PosRewind();
	}

	auto Signer::Sign(const std::span<std::uint8_t> spCheckData, const std::size_t nOrgFileSize, const std::size_t nNewFileSize) -> void
	{
		for (auto& byte : spCheckData)
		{
			byte -= static_cast<std::uint8_t>(nOrgFileSize);
		}

		for (auto& byte : spCheckData)
		{
			byte += static_cast<std::uint8_t>(nNewFileSize);
		}
	}

	auto Signer::GetKeyStr() const -> const std::string&
	{
		return m_msKey;
	}


	auto Signer::SwapDataByteOrder(const std::span<std::uint8_t> spData) -> void
	{
		for (auto& byte : spData)
		{
			std::uint8_t l = (byte << 4) & 0xF0;
			std::uint8_t h = (byte >> 4) & 0x0F;
			byte = l | h;
		}
	}

	auto Signer::DecodeBothEnds(const std::span<std::uint8_t> spData, const std::size_t nTitleLen) -> void
	{
		if (spData.empty()) { return; }
		spData.front() -= static_cast<std::uint8_t>(nTitleLen); // decode first byte
		spData.back() += static_cast<std::uint8_t>(0x2F * (nTitleLen / 0x2F) - nTitleLen); // decode last byte
	}

	auto Signer::EncodeBothEnds(const std::span<std::uint8_t> spData, const std::size_t nTitleLen) -> void
	{
		if (spData.empty()) { return; }
		spData.front() += static_cast<std::uint8_t>(nTitleLen); //encode first byte
		spData.back() -= static_cast<std::uint8_t>(0x2F * (nTitleLen / 0x2F) - nTitleLen); // encode last byte
	}
}