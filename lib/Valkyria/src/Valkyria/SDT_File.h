#pragma once
#include <filesystem>

#include <Rut/RxMem.h>
#include "Valkyria_Types.h"


namespace Valkyria::SDT
{
	class File_Parser
	{
	private:
		VAL_SDT_HDR_Info* m_pInfo = nullptr;
		Rut::RxMem::Auto m_amSDT;

	public:
		File_Parser();
		File_Parser(const std::filesystem::path& phSdt);

		void Load(const std::filesystem::path& phSdt);

	public:
		const VAL_SDT_HDR_Info* GetInfoPtr() const noexcept;
		const Rut::RxMem::Auto& GetMem() const noexcept;
		const size_t GetMsgCount() const noexcept;
		uint8_t* GetSdtPtr() const noexcept;
		uint8_t* GetCodePtr() const noexcept;
		const size_t GetSdtSize() const noexcept;
		const size_t GetCodeSize() const noexcept;
	};
}