#include "RxMem.h"
#include "RxFile.h"

#include <cassert>


namespace Rut
{
	RxMem::RxMem() : m_uiMemSize(0), m_uiMaxSize(0)
	{

	}

	RxMem::RxMem(size_t nSize) :RxMem()
	{
		this->SetSize(nSize);
	}

	RxMem::RxMem(const RxMem& buffer) :RxMem()
	{
		this->Copy(buffer);
	}

	RxMem::RxMem(RxMem&& buffer) noexcept :RxMem()
	{
		this->Move(std::move(buffer));
	}

	RxMem::RxMem(const std::string_view msPath, size_t szFile) : RxMem()
	{
		this->LoadFile(msPath, szFile);
	}

	RxMem::RxMem(const std::wstring_view wsPath, size_t szFile) : RxMem()
	{
		this->LoadFile(wsPath, szFile);
	}

	RxMem::~RxMem()
	{
		m_uiMemSize = 0;
		m_uiMaxSize = 0;
	}

	RxMem& RxMem::Copy(const RxMem& buffer)
	{
		assert(this != &buffer);

		if (buffer.m_upMemData != nullptr)
		{
			this->m_uiMaxSize = buffer.m_uiMaxSize;
			this->m_uiMemSize = buffer.m_uiMemSize;
			this->m_upMemData = std::make_unique_for_overwrite<uint8_t[]>(buffer.m_uiMemSize);
			memcpy(m_upMemData.get(), buffer.m_upMemData.get(), m_uiMemSize);
		}
		else
		{
			this->m_uiMaxSize = 0;
			this->m_uiMemSize = 0;
		}

		return *this;
	}

	RxMem& RxMem::Move(RxMem&& buffer) noexcept
	{
		assert(this != &buffer);

		m_upMemData = std::move(buffer.m_upMemData);

		m_uiMemSize = buffer.m_uiMemSize;
		m_uiMaxSize = buffer.m_uiMaxSize;

		buffer.m_uiMemSize = 0;
		buffer.m_uiMaxSize = 0;

		return *this;
	}

	RxMem& RxMem::Append(const RxMem& rfMem)
	{
		size_t cur_size = this->GetSize();
		size_t append_size = rfMem.GetSize();
		if (append_size)
		{
			this->SetSize(cur_size + append_size, true);
			memcpy(this->GetPtr() + cur_size, rfMem.GetPtr(), append_size);
		}
		return *this;
	}

	uint8_t RxMem::operator[] (size_t nIndex) noexcept
	{
		assert(nIndex < m_uiMemSize);
		return this->GetPtr()[nIndex];
	}

	RxMem& RxMem::operator=(RxMem&& rfAutoMem) noexcept
	{
		return this->Move(std::move(rfAutoMem));
	}

	RxMem& RxMem::operator=(const RxMem& rfAutoMem)
	{
		return this->Copy(rfAutoMem);
	}

	RxMem& RxMem::operator+(const RxMem& rfAutoMem)
	{
		this->Append(rfAutoMem);
		return *this;
	}

	void RxMem::SaveData(const std::string_view msPath)
	{
		RxFile::SaveFileViaPath(msPath, m_upMemData.get(), m_uiMemSize);
	}

	void RxMem::SaveData(const std::wstring_view wsPath)
	{
		RxFile::SaveFileViaPath(wsPath, m_upMemData.get(), m_uiMemSize);
	}

	void RxMem::LoadFile(const std::string_view msPath, size_t nSize)
	{
		RxFile::Binary ifs{ msPath, RIO_READ };
		nSize == AUTO_MEM_AUTO_SIZE ? (void)(nSize = ifs.GetSize()) : (void)(0);
		this->SetSize(nSize);
		ifs.Read(this->GetPtr(), nSize);
	}

	void RxMem::LoadFile(const std::wstring_view wsPath, size_t nSize)
	{
		RxFile::Binary ifs{ wsPath, RIO_READ };
		nSize == AUTO_MEM_AUTO_SIZE ? (void)(nSize = ifs.GetSize()) : (void)(0);
		this->SetSize(nSize);
		ifs.Read(this->GetPtr(), nSize);
	}

	uint8_t* RxMem::GetPtr() const noexcept
	{
		return m_upMemData.get();
	}

	constexpr size_t RxMem::GetSize() const noexcept
	{
		return m_uiMemSize;
	}

	void RxMem::SetSize(size_t uiNewSize, bool isCopy)
	{
		if (m_uiMemSize == 0)
		{
			m_upMemData = std::make_unique_for_overwrite<uint8_t[]>(uiNewSize);
			m_uiMaxSize = uiNewSize;
		}
		else if (uiNewSize > m_uiMaxSize)
		{
			std::unique_ptr<uint8_t[]> tmp = std::make_unique_for_overwrite<uint8_t[]>(uiNewSize);
			isCopy ? (void)(memcpy(tmp.get(), m_upMemData.get(), m_uiMemSize)) : (void)(0);
			m_upMemData = std::move(tmp);
			m_uiMaxSize = uiNewSize;
		}

		m_uiMemSize = uiNewSize;
	}
}