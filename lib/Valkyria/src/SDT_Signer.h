#pragma once
#include <string>
#include "../../Rut/RxMem.h"


namespace Valkyria::SDT
{
	class Signer
	{
	private:
		std::string m_msKey;
		size_t m_uiGameTitleLen = 0;

	private:
		static void SwapDataByteOrder(void* pData, size_t nBytes);
		static void DecodeBothEnds(void* pData, size_t nBytes, size_t nTitleLen);
		static void EncodeBothEnds(void* pData, size_t nBytes, size_t nTitleLen);

	public:
		Signer();
		Signer(std::string_view msKey, size_t nGameTitleLen);
		void InitViaSysData(std::wstring wsSysDataPath);

	public:
		std::string EncodeKey() const;
		std::string DecodeKey(std::string msEncodeDebugKeyStr) const;
		std::string MakeCheckData(size_t nScriptSize) const;
		static std::string MakeCheckData(std::string msOrgCheckData, size_t nOrgSize, size_t nNewSize);
		static void MakeCheckData(uint8_t* pCheckData, size_t nOrgSize, size_t nNewSize);
		const std::string& GetKey() const;
		void Sign(Rut::RxMem::Auto& amSDT) const;
	};
}