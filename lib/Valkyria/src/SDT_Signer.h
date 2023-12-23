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
		Rut::RxMem::Auto EncodeKey() const;
		Rut::RxMem::Auto MakeCheckData(size_t nScriptSize) const;
		void Sign(Rut::RxMem::Auto& amSDT) const;
		static std::string DecodeKey(const uint8_t* pKeyData, size_t nKeySize, size_t nGameTitleLen);
		static void Sign(uint8_t* pOrgCheckData, size_t nOrgCheckDataSize, size_t nOrgFileSize, size_t nNewFileSize);

	public:
		const std::string& GetKeyStr() const;

	};
}