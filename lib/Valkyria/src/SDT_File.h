#pragma once
#include <vector>
#include <stdexcept>

#include "SDT_HDR.h"
#include "SDT_Code.h"

#include "../../Rut/RxMem.h"


namespace Valkyria::SDT
{
	class File_Parser
	{
	private:
		HDR m_HDR;
		Rut::RxMem::Auto m_amCode;

	public:
		File_Parser();
		File_Parser(std::wstring_view wsPath);

		void Parse(std::wstring_view wsPath);
		void Parse(Rut::RxMem::Auto& amSDT);
		Rut::RxMem::Auto Make() const;

	public:
		Rut::RxMem::Auto& GetCodeMem();
		const HDR& GetHDR();
	};
}