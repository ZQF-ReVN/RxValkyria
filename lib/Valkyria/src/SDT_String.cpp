#include "SDT_String.h"
#include "../../Rut/RxStr.h"

#include <stdexcept>


namespace Valkyria::SDT
{
	static std::string MakeUnicodeStrA(size_t wChar)
	{
		char buf[0x10];
		size_t len = (size_t)sprintf_s(buf, 0x10, "\\u%04x", wChar);
		return { buf, len };
	}

	static std::wstring MakeUnicodeStrW(size_t wChar)
	{
		wchar_t buf[0x10];
		size_t len = (size_t)swprintf_s(buf, 0x10, L"\\u%04x", wChar);
		return { buf, len };
	}

	void String::Encode(uint8_t* cpStr)
	{
		while (true)
		{
			(*cpStr) ^= 0xFF;
			if ((*cpStr) == 0xFF) { break; }
			cpStr++;
		}
	}

	std::string String::Decode(uint8_t* const pData)
	{
		std::string text;
		uint8_t* data_ptr = pData;
		while (true)
		{
			uint8_t tmp_char = (*data_ptr ^ 0xFF);
			if (tmp_char == '\0') { break; }
			text.append(1, tmp_char);
			data_ptr++;
		}
		return text;
	}

	std::string String::MakeText(std::wstring_view wsText, size_t nCodePage)
	{
		std::string str_bytes;

		if (nCodePage == 1200)
		{
			for (size_t ite_unit = 0; ite_unit < wsText.size(); ite_unit++)
			{
				wchar_t unit = wsText[ite_unit];
				if (unit == L'\\')
				{
					ite_unit++;
					unit = wsText[ite_unit];
					switch (unit)
					{
					case L'U':
					case L'u':
					{
						uint32_t code_point = 0;
						swscanf_s(wsText.data() + ite_unit + 1, L"%04x", &code_point);
						str_bytes.append(MakeUnicodeStrA(code_point));
						ite_unit += 4;
					}
					break;

					case L'R':
					case L'r':
					{
						str_bytes.append(1, '\\');
						str_bytes.append(1, 'r');
					}
					break;

					case L'\\':
					{
						throw std::runtime_error("Unknow Format");
					}
					break;

					default: throw std::runtime_error("Unknow Format");
					}
				}
				else
				{
					str_bytes.append(MakeUnicodeStrA(unit));
				}
			}
		}
		else if (nCodePage == 936)
		{
			std::wstring format_text;
			for (auto& unit : wsText)
			{
				switch (unit)
				{
				case L'・':case L'≪':case L'≫':case L'♪':format_text.append(MakeUnicodeStrW(unit)); break;
				default: format_text.append(1, unit);
				}
			}
			str_bytes = Rut::RxStr::ToMBCS(format_text, nCodePage);
		}
		else
		{
			str_bytes = Rut::RxStr::ToMBCS(wsText, nCodePage);
		}

		if (str_bytes.size() >= 260) 
		{ 
			throw std::runtime_error("EncodeString: exceeds buffer size limit");
		}

		return str_bytes;
	}

	std::wstring String::LoadText(std::string_view msText, size_t nCodePage)
	{
		return Rut::RxStr::ToWCS(msText, nCodePage);
	}

	std::wstring String::NumToStr(const wchar_t* wpFormat, size_t nValue)
	{
		wchar_t buf[0x10];
		size_t len = (size_t)swprintf_s(buf, 0x10, wpFormat, nValue);
		return { buf, len };
	}

	size_t String::StrToNum(const wchar_t* wpFormat, std::wstring_view wsText)
	{
		size_t value = 0;
		size_t len = (size_t)swscanf_s(wsText.data(), wpFormat, &value);
		return value;
	}
}