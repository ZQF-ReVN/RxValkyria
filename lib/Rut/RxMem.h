#pragma once
#include <string>
#include <memory>


namespace Rut
{
	constexpr size_t AUTO_MEM_AUTO_SIZE = -1;

	class RxMem
	{
	private:
		size_t m_uiMemSize;
		size_t m_uiMaxSize;
		std::unique_ptr<uint8_t[]> m_upMemData;

	public:
		RxMem();
		RxMem(size_t nSize);
		RxMem(const RxMem& buffer);
		RxMem(RxMem&& buffer) noexcept;
		RxMem(const std::string_view msPath, size_t szFile = AUTO_MEM_AUTO_SIZE);
		RxMem(const std::wstring_view wsPath, size_t szFile = AUTO_MEM_AUTO_SIZE);
		~RxMem();

		RxMem& Copy(const RxMem& buffer);
		RxMem& Move(RxMem&& buffer) noexcept;
		RxMem& Append(const RxMem& rfMem);

		uint8_t operator[] (size_t tSize) noexcept;
		RxMem& operator=(RxMem&& rfAutoMem) noexcept;
		RxMem& operator=(const RxMem& rfAutoMem);
		RxMem& operator+(const RxMem& rfAutoMem);

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