#pragma once
#include <string>

#include "Valkyria_Types.h"
#include "../../Rut/RxMem.h"


namespace Valkyria::SDT
{
	class Checker
	{
	private:
		std::string m_msKey;
		size_t m_uiGameTitleLen = 0;

	private:
		static void SwapStrByteOrder(void* pData, size_t nBytes)
		{
			uint8_t* data_ptr = (uint8_t*)pData;

			for (size_t ite_byte = 0; ite_byte < nBytes; ite_byte++)
			{
				uint8_t low = (data_ptr[ite_byte] << 4) & 0xF0;
				uint8_t hight = (data_ptr[ite_byte] >> 4) & 0x0F;
				data_ptr[ite_byte] = low | hight;
			}
		}

		static void DecodeBothEnds(void* pData, size_t nBytes, size_t nTitleLen)
		{
			uint8_t* data_ptr = (uint8_t*)pData;
			data_ptr[0] -= (uint8_t)nTitleLen; 	// decode first byte
			data_ptr[nBytes - 1] += (uint8_t)(0x2F * (nTitleLen / 0x2F) - nTitleLen); // decode last byte
		}

		static void EncodeBothEnds(void* pData, size_t nBytes, size_t nTitleLen)
		{
			uint8_t* data_ptr = (uint8_t*)pData;
			data_ptr[0] += (uint8_t)nTitleLen; //encode first byte
			data_ptr[nBytes - 1] -= (uint8_t)(0x2F * (nTitleLen / 0x2F) - nTitleLen); // encode last byte
		}


	public:
		Checker()
		{

		}

		Checker(std::string_view msKey, size_t nGameTitleLen)
		{
			m_msKey = msKey;
			m_uiGameTitleLen = nGameTitleLen;
		}

		void InitViaSysData(std::wstring wsSysDataPath)
		{
			Rut::RxMem::Auto sys_data{ wsSysDataPath };
			VAL_SysData_Seg_0* pSeg0 = (VAL_SysData_Seg_0*)(sys_data.GetPtr());
			m_uiGameTitleLen = strlen(pSeg0->aGameTitle);
			m_msKey = this->DecodeKey(pSeg0->aCheckKey);
		}

		std::string EncodeKey()
		{
			std::string enc_debug_key = m_msKey;
			this->SwapStrByteOrder(enc_debug_key.data(), enc_debug_key.size());
			this->EncodeBothEnds(enc_debug_key.data(), enc_debug_key.size(), m_uiGameTitleLen);
			return enc_debug_key;
		}

		std::string DecodeKey(std::string msEncodeDebugKeyStr)
		{
			this->DecodeBothEnds(msEncodeDebugKeyStr.data(), msEncodeDebugKeyStr.size(), m_uiGameTitleLen);
			this->SwapStrByteOrder(msEncodeDebugKeyStr.data(), msEncodeDebugKeyStr.size());
			return msEncodeDebugKeyStr;
		}

		std::string MakeCheckData(size_t nScriptSize)
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
	};
}