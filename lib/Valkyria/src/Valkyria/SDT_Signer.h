#pragma once
#include <span>
#include <string>

#include <Rut/RxMem.h>


namespace Valkyria::SDT
{
	class Signer
	{
	private:
		std::string m_msKey;
		size_t m_uiGameTitleLen = 0;

	private:
		static void SwapDataByteOrder(std::span<uint8_t> spData);
		static void DecodeBothEnds(std::span<uint8_t> spData, size_t nTitleLen);
		static void EncodeBothEnds(std::span<uint8_t> spData, size_t nTitleLen);

	public:
		Signer();
		Signer(std::string_view msKey, size_t nGameTitleLen);
		void InitViaSysData(std::wstring wsSysDataPath);

	public:
		Rut::RxMem::Auto EncodeKey() const;
		Rut::RxMem::Auto MakeCheckData(size_t nScriptSize) const;
		void Sign(Rut::RxMem::Auto& amSDT) const;
		static std::string DecodeKey(std::span<uint8_t> spKeyData, size_t nGameTitleLen);
		static void Sign(std::span<uint8_t> spCheckData, size_t nOrgFileSize, size_t nNewFileSize);

	public:
		const std::string& GetKeyStr() const;

	};
}