#include <RxValkyria/Core/SDT_String.h>
#include <ZxCvt/ZxCvt.h>
#include <stdexcept>


namespace ZQF::RxValkyria::SDT
{
	auto String::Encode(std::uint8_t* cpStr) -> void
	{
		while (true)
		{
			(*cpStr) ^= 0xFF;
			if ((*cpStr) == 0xFF) { break; }
			cpStr++;
		}
	}

	auto String::Decode(const std::uint8_t* const pData) -> std::string
	{
		std::string text;
		const std::uint8_t* data_ptr = pData;
		while (true)
		{
			std::uint8_t tmp_char = (*data_ptr ^ 0xFF);
			if (tmp_char == '\0') { break; }
			text.append(1, tmp_char);
			data_ptr++;
		}
		return text;
	}

	auto String::MakeText(const std::string_view msText, const std::size_t nCodePage) -> std::string
	{
		ZxCvt cvt;
		std::string str_bytes;

		if (nCodePage == 936)
		{
			const auto u16_text = cvt.UTF8ToUTF16LE(msText);
			std::u16string format_text;
			for (auto unit : u16_text)
			{
				switch (unit)
				{
				case u'・': format_text.append(u"\\u30fb"); break;
				case u'≪': format_text.append(u"\\u226a"); break;
				case u'≫': format_text.append(u"\\u226b"); break;
				case u'♪': format_text.append(u"\\u266a"); break;
				case u'⇒': format_text.append(u"\\u21D2"); break;
				default: format_text.append(1, unit);
				}
			}
			str_bytes = cvt.UTF16LEToMBCS(format_text, nCodePage);
		}
		else
		{
			str_bytes = cvt.UTF8ToMBCS(msText, nCodePage);
		}

		if (str_bytes.size() >= 260)
		{
			throw std::runtime_error("EncodeString: exceeds buffer size limit");
		}

		return str_bytes;
	}

	auto String::LoadText(const std::string_view msText, const std::size_t nCodePage) -> std::string
	{
		ZxCvt cvt;
		return std::string{ cvt.MBCSToUTF8(msText, nCodePage) };
	}

	auto String::NumToStr(const char* cpFormat, const std::size_t nValue) -> std::string
	{
		char buf[0x10];
		const auto len = static_cast<std::size_t>(std::snprintf(buf, 0x10, cpFormat, nValue));
		return { buf, len };
	}

	auto String::StrToNum(const char* cpFormat, const std::string_view wsText) -> std::size_t
	{
		std::size_t value{};
#ifdef _WIN32
		const auto len = static_cast<std::size_t>(sscanf_s(wsText.data(), cpFormat, &value));
#else
		const auto len = static_cast<std::size_t>(std::sscanf(wsText.data(), cpFormat, &value));
#endif // _WIN32
		if (len == 0) { throw std::runtime_error("RxValkyria::SDT::String::StrToNum(): error!"); }
		return value;
	}
}