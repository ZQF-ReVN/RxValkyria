#include "SDT_File.h"


namespace Valkyria::SDT
{
	
	File_Parser::File_Parser()
	{

	}

	File_Parser::File_Parser(std::wstring_view wsPath)
	{
		this->Parse(wsPath);
	}

	void File_Parser::Parse(std::wstring_view wsPath)
	{
		Rut::RxMem::Auto sdt_buffer(wsPath);
		this->Parse(sdt_buffer);
	}

	void File_Parser::Parse(Rut::RxMem::Auto& amSDT)
	{
		m_HDR.Parse(amSDT.GetPtr());

		size_t code_seg_size = amSDT.GetSize() - m_HDR.GetSize();
		uint8_t* code_seg_ptr = amSDT.GetPtr() + m_HDR.GetSize();
		m_amCode.SetSize(code_seg_size);

		memcpy(m_amCode.GetPtr(), code_seg_ptr, code_seg_size);
	}

	Rut::RxMem::Auto File_Parser::Make() const
	{
		return { m_HDR.Make(), m_amCode };
	}

	Rut::RxMem::Auto& File_Parser::GetCodeMem()
	{
		return m_amCode;
	}

	const HDR& File_Parser::GetHDR()
	{
		return m_HDR;
	}
}