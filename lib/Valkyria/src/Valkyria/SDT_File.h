#pragma once
#include "Valkyria_Types.h"
#include <Rut/RxMem.h>


namespace Valkyria::SDT
{
	class File_Parser
	{
	private:
		VAL_SDT_HDR_Info* m_pInfo = nullptr;
		Rut::RxMem::Auto m_amSDT;

	public:
		File_Parser()
		{

		}

		File_Parser(std::wstring_view wsPath)
		{
			
		}

		void Load(std::wstring_view wsPath)
		{
			m_amSDT.LoadFile(wsPath);
			m_pInfo = (VAL_SDT_HDR_Info*)this->GetSdtPtr();
		}

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