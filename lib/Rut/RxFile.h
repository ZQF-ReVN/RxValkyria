#pragma once
#include <string>
#include <sstream>
#include <functional>


namespace Rut
{
	constexpr size_t RIO_DATA_IN = 0x01;
	constexpr size_t RIO_DATA_OUT = 0x02;
	constexpr size_t RIO_CREATE_ALWAYS = 0x04;
	constexpr size_t RIO_CREATE_NEW = 0x08;
	constexpr size_t RIO_OPEN_ALWAYS = 0x10;
	constexpr size_t RIO_OPEN_EXISTING = 0x20;

	constexpr size_t RIO_BEGIN = 0x0;
	constexpr size_t RIO_CURRENT = 0x1;
	constexpr size_t RIO_END = 0x2;

	constexpr size_t RIO_READ = RIO_DATA_IN | RIO_OPEN_EXISTING;
	constexpr size_t RIO_WRITE = RIO_DATA_OUT | RIO_CREATE_ALWAYS;
}


namespace Rut::RxFile
{
	class Basic
	{
	protected:
		void* m_hFile;

	protected:
		Basic();
		Basic(Basic&& refStream) noexcept;
		~Basic();

	public:
		void Create(std::string_view msPath, size_t nMode);
		void Create(std::wstring_view wsPath, size_t nMode);

		bool Close();
		bool Flush();
		bool IsEnd();

		size_t GetPos();
		size_t GetSize();
		size_t SetPos(size_t nOffset, size_t nMode);
		size_t Read(void* pBuffer, size_t nSize);
		size_t Write(void* pData, size_t nSize);

	};
}

namespace Rut::RxFile
{
	class Binary : public Basic
	{
	public:
		Binary();
		Binary(std::string_view msPath, size_t nMode);
		Binary(std::wstring_view wsPath, size_t nMode);
		Binary(const Binary& refStream) = delete;

	public:
		template <typename T_TYPE> operator T_TYPE()
		{
			T_TYPE tmp{ 0 };
			this->Read((T_TYPE*)&tmp, sizeof(T_TYPE));
			return tmp;
		}

		template <typename T_TYPE> Binary& operator >>(T_TYPE& TYPE)
		{
			this->Read((void*)&TYPE, sizeof(TYPE));
			return *this;
		}

		template <typename T_TYPE> Binary& operator <<(T_TYPE& TPYE)
		{
			this->Write((void*)&TPYE, sizeof(TPYE));
			return *this;
		}
	};

	void SaveFileViaPath(std::wstring_view wsPath, void* pData, size_t nBytes);
	void SaveFileViaPath(std::string_view msPath, void* pData, size_t nBytes);
}


namespace Rut
{
	constexpr size_t RFM_ANSI = 0x0;
	constexpr size_t RFM_UTF8 = 0x1;
	constexpr size_t RFM_UTF16 = 0x2;
}

namespace Rut::RxFile
{
	class Text : private Binary
	{
	private:
		size_t m_rxFormat;

	public:
		Text();
		Text(std::string_view msPath, size_t nMode, size_t nFormat);
		Text(std::wstring_view wsPath, size_t nMode, size_t nFormat);

		Text& operator <<(std::string_view msStr) { WriteLine(msStr); return *this; }
		Text& operator <<(std::wstring_view wsStr) { WriteLine(wsStr); return *this; }

		void WriteBOM();
		void CheckBOM();
		void EnsureBom(size_t nMode);

		size_t WriteLine(std::string_view msStr);
		size_t WriteLine(std::wstring_view wsStr);

		void WriteAllLine(std::vector<std::wstring>& vecLine);
		void WriteAllLine(std::vector<std::string>& vecLine);

		void ReadAllLine(std::vector<std::string>& vecLine);
		void ReadAllLine(std::vector<std::wstring>& vecLine);
		void ReadAllLine(std::function<bool(char*, char*)> fnPerline);
		void ReadAllLine(std::function<bool(wchar_t*, wchar_t*)> fnPerline);

		void ReadRawText(std::string& msText);
		void ReadRawText(std::wstring& wsText);

		void ReadToSStream(std::wstringstream& rfSStream);
		void ReadToSStream(std::stringstream& rfSStream);

		void Flush();
		void Close();

		void Rewind();
	};
}
