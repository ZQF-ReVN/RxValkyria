#include "SDT_Signer.h"
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
		m_msKey = this->DecodeKey((uint8_t*)pSeg0->aCheckKey, strlen(pSeg0->aCheckKey) + 1, m_uiGameTitleLen);
	}

	Rut::RxMem::Auto Signer::EncodeKey() const
	{
		Rut::RxMem::Auto enc_key_mem(m_msKey.size() + 1);
		memcpy(enc_key_mem.GetPtr(), m_msKey.data(), m_msKey.size() + 1);
		this->SwapDataByteOrder(enc_key_mem.GetPtr(), enc_key_mem.GetSize() - 1);
		this->EncodeBothEnds(enc_key_mem.GetPtr(), enc_key_mem.GetSize() - 1, m_uiGameTitleLen);
		return enc_key_mem;
	}

	Rut::RxMem::Auto Signer::MakeCheckData(size_t nScriptSize) const
	{
		if (m_msKey.empty()) { throw std::runtime_error("SDT::Signer: Error Not Key Data!"); }

		Rut::RxMem::Auto check_data_mem = this->EncodeKey();

		uint8_t* check_data_ptr = check_data_mem.GetPtr();
		size_t check_data_size = check_data_mem.GetSize();

		for (size_t ite_byte = 0; ite_byte < check_data_size; ite_byte++)
		{
			check_data_ptr[ite_byte] += (uint8_t)nScriptSize;
		}

		return check_data_mem;
	}

	void Signer::Sign(Rut::RxMem::Auto& amSDT) const
	{
		Rut::RxMem::Auto check_mem = this->MakeCheckData(amSDT.GetSize());

		VAL_SDT_HDR_Info* info_ptr = (VAL_SDT_HDR_Info*)amSDT.GetPtr();
		size_t check_data_size = info_ptr->uiHDRSize - info_ptr->uiCheckDataFOA;
		if (check_data_size != check_mem.GetSize()) { throw std::runtime_error("Signer::Sign: check data size mismatched!"); }

		uint8_t* check_data_ptr = amSDT.GetPtr() + info_ptr->uiCheckDataFOA;
		memcpy(check_data_ptr, check_mem.GetPtr(), check_data_size);
	}


	std::string Signer::DecodeKey(const uint8_t* pKeyData, size_t nKeyDataSize, size_t nGameTitleLen)
	{
		Rut::RxMem::Auto dec_key_mem(nKeyDataSize);
		memcpy(dec_key_mem.GetPtr(), pKeyData, nKeyDataSize);
		DecodeBothEnds(dec_key_mem.GetPtr(), dec_key_mem.GetSize() - 1, nGameTitleLen);
		SwapDataByteOrder(dec_key_mem.GetPtr(), dec_key_mem.GetSize() - 1);
		return { (char*)dec_key_mem.GetPtr(), dec_key_mem.GetSize() - 1 };
	}

	void Signer::Sign(uint8_t* pOrgCheckData, size_t nOrgCheckDataSize, size_t nOrgFileSize, size_t nNewFileSize)
	{
		for (size_t ite_byte = 0; ite_byte < nOrgCheckDataSize; ite_byte++)
		{
			pOrgCheckData[ite_byte] -= (uint8_t)nOrgFileSize;
		}

		for (size_t ite_byte = 0; ite_byte < nOrgCheckDataSize; ite_byte++)
		{
			pOrgCheckData[ite_byte] += (uint8_t)nNewFileSize;
		}
	}

	const std::string& Signer::GetKeyStr() const
	{
		return m_msKey;
	}
}