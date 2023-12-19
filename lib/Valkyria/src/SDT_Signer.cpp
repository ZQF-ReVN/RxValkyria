#include "SDT_Signer.h"
#include "SDT_Parser.h"
#include "Valkyria_Types.h"
#include "../../Rut/RxMem.h"

#include <stdexcept>


namespace Valkyria::SDT
{
	void Signer::SwapDataByteOrder(void* pData, size_t nBytes)
	{
		uint8_t* data_ptr = (uint8_t*)pData;

		for (size_t ite_byte = 0; ite_byte < nBytes; ite_byte++)
		{
			uint8_t low = (data_ptr[ite_byte] << 4) & 0xF0;
			uint8_t hight = (data_ptr[ite_byte] >> 4) & 0x0F;
			data_ptr[ite_byte] = low | hight;
		}
	}

	void Signer::DecodeBothEnds(void* pData, size_t nBytes, size_t nTitleLen)
	{
		uint8_t* data_ptr = (uint8_t*)pData;
		data_ptr[0] -= (uint8_t)nTitleLen; 	// decode first byte
		data_ptr[nBytes - 1] += (uint8_t)(0x2F * (nTitleLen / 0x2F) - nTitleLen); // decode last byte
	}

	void Signer::EncodeBothEnds(void* pData, size_t nBytes, size_t nTitleLen)
	{
		uint8_t* data_ptr = (uint8_t*)pData;
		data_ptr[0] += (uint8_t)nTitleLen; //encode first byte
		data_ptr[nBytes - 1] -= (uint8_t)(0x2F * (nTitleLen / 0x2F) - nTitleLen); // encode last byte
	}

	Signer::Signer()
	{

	}

	Signer::Signer(std::string_view msKey, size_t nGameTitleLen)
	{
		m_msKey = msKey;
		m_uiGameTitleLen = nGameTitleLen;
	}

	// VAL-0064 VAL-0065 VAL-0066 VAL-0017Re
	void Signer::InitViaSysData(std::wstring wsSysDataPath)
	{
		Rut::RxMem::Auto sys_data{ wsSysDataPath };
		VAL_SysData_Seg_0* pSeg0 = (VAL_SysData_Seg_0*)(sys_data.GetPtr());
		m_uiGameTitleLen = strlen(pSeg0->aGameTitle);
		m_msKey = this->DecodeKey(pSeg0->aCheckKey);
	}

	std::string Signer::EncodeKey() const
	{
		std::string enc_debug_key = m_msKey;
		this->SwapDataByteOrder(enc_debug_key.data(), enc_debug_key.size());
		this->EncodeBothEnds(enc_debug_key.data(), enc_debug_key.size(), m_uiGameTitleLen);
		return enc_debug_key;
	}

	std::string Signer::DecodeKey(std::string msEncodeDebugKeyStr) const
	{
		this->DecodeBothEnds(msEncodeDebugKeyStr.data(), msEncodeDebugKeyStr.size(), m_uiGameTitleLen);
		this->SwapDataByteOrder(msEncodeDebugKeyStr.data(), msEncodeDebugKeyStr.size());
		return msEncodeDebugKeyStr;
	}

	std::string Signer::MakeCheckData(size_t nScriptSize) const
	{
		if (m_msKey.empty()) { return ""; }

		std::string check_data = this->EncodeKey();
		this->DecodeBothEnds(check_data.data(), check_data.size(), m_uiGameTitleLen);

		check_data.front() += (uint8_t)m_uiGameTitleLen;
		check_data.back() -= (uint8_t)(0x2F * (m_uiGameTitleLen / 0x2F) - m_uiGameTitleLen);

		for (auto& byte : check_data)
		{
			byte += (uint8_t)nScriptSize;
		}

		return check_data;
	}

	const std::string& Signer::GetKey() const
	{
		return m_msKey;
	}

	void Signer::Sign(Rut::RxMem::Auto& amSDT) const
	{
		std::string sig_data = this->MakeCheckData(amSDT.GetSize());

		SDT::HDR hdr;
		hdr.Parse(amSDT.GetPtr());

		size_t check_data_size = hdr.GetCheckDataSize();
		if ((check_data_size - 1) != sig_data.size()) { throw std::runtime_error("Signer::Sign: check data size mismatched!"); }

		hdr.SetCheckData(sig_data);

		Rut::RxMem::Auto hdr_mem = hdr.Make();
		memcpy(amSDT.GetPtr(), hdr_mem.GetPtr(), hdr_mem.GetSize());
	}
}