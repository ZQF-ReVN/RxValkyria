#include "SDT_File.h"


namespace Valkyria::SDT
{
	const VAL_SDT_HDR_Info* File_Parser::GetInfoPtr() const noexcept
	{
		return m_pInfo;
	}

	const Rut::RxMem::Auto& File_Parser::GetMem() const noexcept
	{
		return m_amSDT;
	}

	const size_t File_Parser::GetMsgCount() const noexcept
	{
		return m_pInfo->uiMsgCount;
	}

	uint8_t* File_Parser::GetSdtPtr() const noexcept
	{
		return m_amSDT.GetPtr();
	}

	uint8_t* File_Parser::GetCodePtr() const noexcept
	{
		return m_amSDT.GetPtr() + m_pInfo->uiHDRSize;
	}

	const size_t File_Parser::GetSdtSize() const noexcept
	{
		return m_amSDT.GetSize();
	}

	const size_t File_Parser::GetCodeSize() const noexcept
	{
		return m_amSDT.GetSize() - m_pInfo->uiHDRSize;
	}
}