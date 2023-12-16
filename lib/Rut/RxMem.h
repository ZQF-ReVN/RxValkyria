#pragma once
#include <string>
#include <memory>


namespace Rut::RxMem
{
	constexpr size_t AUTO_MEM_AUTO_SIZE = -1;

	class Auto
	{
	private:
		size_t m_uiMemSize;
		size_t m_uiMaxSize;
		std::unique_ptr<uint8_t[]> m_upMemData;

	public:
		Auto();
		Auto(std::initializer_list<Auto> lsOBJ);
		Auto(size_t nSize);
		Auto(const Auto& buffer);
		Auto(Auto&& buffer) noexcept;
		Auto(const std::string_view msPath, size_t szFile = AUTO_MEM_AUTO_SIZE);
		Auto(const std::wstring_view wsPath, size_t szFile = AUTO_MEM_AUTO_SIZE);
		~Auto();

		Auto& Copy(const Auto& buffer);
		Auto& Move(Auto&& buffer) noexcept;
		Auto& Append(const Auto& rfMem);

		uint8_t operator[] (size_t tSize) noexcept;
		Auto& operator=(Auto&& rfAutoMem) noexcept;
		Auto& operator=(const Auto& rfAutoMem);
		Auto& operator+(const Auto& rfAutoMem);

	public:
		void SaveData(const std::string_view msPath);
		void SaveData(const std::wstring_view wsPath);
		void LoadFile(const std::string_view msPath, size_t nSize = AUTO_MEM_AUTO_SIZE);
		void LoadFile(const std::wstring_view wsPath, size_t nSize = AUTO_MEM_AUTO_SIZE);

	public:
		uint8_t* GetPtr() const noexcept;
		constexpr size_t GetSize() const noexcept;
		void SetSize(size_t uiNewSize, bool isCopy = false);
	};
}