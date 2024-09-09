#pragma once
#include <span>
#include <string>
#include <Zut/ZxMem.h>


namespace ZQF::ReVN::RxValkyria::SDT
{
	class Signer
	{
	private:
		std::string m_msKey;
		std::size_t m_uiGameTitleLen{};

	public:
		Signer();
		Signer(const std::string_view msKey, const std::size_t nGameTitleLen);

	public:
		auto InitViaSysData(const std::string_view msSysDataPath) -> void;

	public:
		auto EncodeKey() const -> ZxMem;
		auto MakeCheckData(const std::size_t nScriptSiz) const->ZxMem;
		static auto DecodeKey(const std::span<std::uint8_t> spKeyData, const std::size_t nGameTitleLen) -> std::string;

	public:
		auto Sign(ZxMem& amSDT) const -> void;
		static auto Sign(const std::span<std::uint8_t> spCheckData, const std::size_t nOrgFileSize, const std::size_t nNewFileSize) -> void;

	public:
		auto GetKeyStr() const -> const std::string&;

	private:
		static auto SwapDataByteOrder(const std::span<std::uint8_t> spData) -> void;
		static auto DecodeBothEnds(const std::span<std::uint8_t> spData, const std::size_t nTitleLen) -> void;
		static auto EncodeBothEnds(const std::span<std::uint8_t> spData, const std::size_t nTitleLen) -> void;
	};
}