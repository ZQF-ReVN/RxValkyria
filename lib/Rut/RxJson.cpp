#include "RxJson.h"
#include "RxFile.h"

#include <assert.h>
#include <stdexcept>
#include <map>

namespace Rut::RxJson
{
	Value::Value()
	{
		m_Type = JVALUE_NUL;
		m_Value = { 0 };
	}

	Value::Value(bool bValue)
	{
		m_Type = JVALUE_BOL;
		m_Value.Bool = bValue;
	}

	Value::Value(int iValue)
	{
		m_Type = JVALUE_INT;
		m_Value.Int = iValue;
	}

	Value::Value(double dValue)
	{
		m_Type = JVALUE_DBL;
		m_Value.Double = dValue;
	}

	Value::Value(const wchar_t* sValue)
	{
		m_Type = JVALUE_STR;
		m_Value.pStr = new JString(sValue);
	}

	Value::Value(std::wstring_view sValue)
	{
		m_Type = JVALUE_STR;
		m_Value.pStr = new JString(sValue);
	}

	Value::Value(const std::wstring& sValue)
	{
		m_Type = JVALUE_STR;
		m_Value.pStr = new JString(sValue);
	}

	Value::Value(std::wstring&& sValue)
	{
		m_Type = JVALUE_STR;
		m_Value.pStr = new JString(std::move(sValue));
	}

	Value::Value(const JArray& aValue)
	{
		m_Type = JVALUE_ARY;
		m_Value.pAry = new JArray(aValue);
	}

	Value::Value(JArray&& aValue)
	{
		m_Type = JVALUE_ARY;
		m_Value.pAry = new JArray(std::move(aValue));
	}

	Value::Value(const JObject& oValue)
	{
		m_Type = JVALUE_OBJ;
		m_Value.pObj = new JObject(oValue);
	}

	Value::Value(JObject&& oValue)
	{
		m_Type = JVALUE_OBJ;
		m_Value.pObj = new JObject(std::move(oValue));
	}

	Value::Value(const Value& rfJValue)
	{
		m_Type = JVALUE_NUL;
		Copy(rfJValue);
	}

	Value::Value(Value&& rfJValue) noexcept
	{
		m_Type = JVALUE_NUL;
		Move(rfJValue);
	}

	Value::~Value()
	{
		switch (m_Type)
		{
		case JValue_Type::JVALUE_STR:
		{
			delete m_Value.pStr;
		}
		break;

		case JValue_Type::JVALUE_ARY:
		{
			delete m_Value.pAry;
		}
		break;

		case JValue_Type::JVALUE_OBJ:
		{
			delete m_Value.pObj;
		}
		break;
		}
	}


	Value& Value::Copy(const Value& rfJValue)
	{
		this->m_Type = rfJValue.m_Type;
		this->m_Value = rfJValue.m_Value;

		switch (m_Type)
		{
		case JValue_Type::JVALUE_STR:
			this->m_Value.pStr = new JString(*(rfJValue.m_Value.pStr)); break;
		case JValue_Type::JVALUE_ARY:
			this->m_Value.pAry = new JArray(*(rfJValue.m_Value.pAry)); break;
		case JValue_Type::JVALUE_OBJ:
			this->m_Value.pObj = new JObject(*(rfJValue.m_Value.pObj)); break;
		}

		return *this;
	}

	Value& Value::Move(Value& rfJValue)
	{
		this->m_Type = rfJValue.m_Type;
		this->m_Value = rfJValue.m_Value;
		rfJValue.m_Type = JVALUE_NUL;
		rfJValue.m_Value = { 0 };
		return *this;
	}


	Value& Value::operator = (const Value& rfJValue)
	{
		this->~Value();
		return Copy(rfJValue);
	}

	Value& Value::operator = (Value&& rfJValue) noexcept
	{
		this->~Value();
		return Move(rfJValue);
	}

	//Array
	void Value::SureArray()
	{
		if (m_Type == JVALUE_NUL)
		{
			m_Type = JVALUE_ARY;
			m_Value.pAry = new JArray();
		}

		if (m_Type != JVALUE_ARY)
		{
			throw std::runtime_error("Error Json Value Not Array!");
		}
	}

	void Value::Append(const Value& rfJValue)
	{
		SureArray();
		m_Value.pAry->push_back(rfJValue);
	}

	void Value::Append(Value&& rfJValue)
	{
		SureArray();
		m_Value.pAry->emplace_back(std::move(rfJValue));
	}


	// Obj
	Value& Value::operator[](const wchar_t* wpKey)
	{
		JObject& obj = this->ToOBJ();
		return (*this->m_Value.pObj)[wpKey];
	}

	void Value::SureObject()
	{
		if (m_Type == JVALUE_NUL)
		{
			m_Type = JVALUE_OBJ;
			m_Value.pObj = new JObject();
		}

		if (m_Type != JVALUE_OBJ)
		{
			throw std::runtime_error("Error Json Value Not Object!");
		}
	}

	void Value::AddKey(std::wstring_view wsKey)
	{
		SureObject();
		(*this->m_Value.pObj)[std::move(std::wstring(wsKey))];
	}

	void Value::AddKey(std::wstring_view wsKey, const Value& rfJValue)
	{
		SureObject();
		(*this->m_Value.pObj)[std::move(std::wstring(wsKey))] = rfJValue;
	}

	void Value::AddKey(std::wstring_view wsKey, Value&& rfJValue)
	{
		SureObject();
		(*this->m_Value.pObj)[std::move(std::wstring(wsKey))] = std::move(rfJValue);
	}

	bool Value::HasKey(std::wstring_view wsKey)
	{
		auto ite = m_Value.pObj->find(std::wstring(wsKey));
		return ite == m_Value.pObj->end() ? false : true;
	}


	JValue_Type Value::GetType()
	{
		return m_Type;
	}

	Value::operator bool()
	{
		if (m_Type == JVALUE_BOL) { return m_Value.Bool; }
		throw std::runtime_error("Error Json Type!");
	}

	Value::operator int()
	{
		if (m_Type == JVALUE_INT) { return m_Value.Int; }
		throw std::runtime_error("Error Json Type!");
	}

	Value::operator double()
	{
		if (m_Type == JVALUE_DBL) { return m_Value.Double; }
		throw std::runtime_error("Error Json Type!");
	}

	Value::operator JString& ()
	{
		if (m_Type == JVALUE_STR) { return *(m_Value.pStr); }
		throw std::runtime_error("Error Json Type!");
	}

	Value::operator JArray& ()
	{
		return this->ToAry();
		throw std::runtime_error("Error Json Type!");
	}

	Value::operator JObject& ()
	{
		return this->ToOBJ();
		throw std::runtime_error("Error Json Type!");
	}

	Value::operator std::wstring_view()
	{
		const std::wstring& str = this->operator JString & ();
		return str;
	}


	JArray& Value::ToAry()
	{
		SureArray();
		return *m_Value.pAry;
	}

	JObject& Value::ToOBJ()
	{
		SureObject();
		return *(m_Value.pObj);
	}

	int Value::ToInt()
	{
		return this->operator int();
	}

	bool Value::ToBool()
	{
		return this->operator bool();
	}

	double Value::ToDouble()
	{
		return this->operator double();
	}

	const wchar_t* Value::ToStringPtr()
	{
		return this->operator std::wstring_view().data();
	}

	std::wstring_view Value::ToStringView()
	{
		return this->operator std::wstring_view();
	}


	void Value::Dump(std::wstring& wsText, bool isFormat, bool isOrder) const
	{
		static size_t count = 0;

		switch (m_Type)
		{
		case JValue_Type::JVALUE_NUL:
		{
			wsText.append(L"null");
		}
		break;

		case JValue_Type::JVALUE_BOL:
		{
			wsText.append((m_Value.Bool) ? L"true" : L"false");
		}
		break;

		case JValue_Type::JVALUE_INT:
		{
			wsText.append(std::to_wstring(m_Value.Int));
		}
		break;

		case JValue_Type::JVALUE_DBL:
		{
			wsText.append(std::to_wstring(m_Value.Double));
		}
		break;

		case JValue_Type::JVALUE_STR:
		{
			wsText.append(1, L'\"');
			for (auto ch : *(m_Value.pStr))
			{
				switch (ch)
				{
				case L'\n': ch = L'n'; break;
				case L'\r': ch = L'r'; break;
				case L'\b': ch = L'b'; break;
				case L'\t': ch = L't'; break;
				case L'\f': ch = L'f'; break;
				case L'\"': ch = L'"'; break;
				case L'\\': ch = L'\\'; break;
				default:
				{
					wsText.append(1, ch);
					continue;
				}
				}

				wsText.append(1, L'\\');
				wsText.append(1, ch);
			}
			wsText.append(1, L'\"');
		}
		break;

		case JValue_Type::JVALUE_ARY:
		{
			wsText.append(1, L'[');

			count++;
			{
				for (auto& value : *(m_Value.pAry))
				{
					if (isFormat)
					{
						wsText.append(1, L'\n');
						wsText.append(count, L'\t');
					}
					value.Dump(wsText, isFormat);
					wsText.append(1, L',');
				}
			}
			count--;

			// if not null array
			if (wsText.back() == L',')
			{
				wsText.pop_back();
				if (isFormat)
				{
					wsText.append(1, L'\n');
					wsText.append(count, L'\t');
				}
			}

			wsText.append(1, L']');
		}
		break;

		case JValue_Type::JVALUE_OBJ:
		{
			wsText.append(1, L'{');

			count++;
			{
				auto fn_proce_one = [&wsText,&isFormat](const std::pair<const std::wstring, Value>& rfKV)
					{
						if (isFormat)
						{
							wsText.append(1, L'\n');
							wsText.append(count, L'\t');
						}
						wsText.append(1, L'\"');
						wsText.append(rfKV.first);
						wsText.append(1, L'\"');
						wsText.append(1, L':');
						if (isFormat)
						{
							wsText.append(1, L' ');
						}
						rfKV.second.Dump(wsText, isFormat);
						wsText.append(1, L',');
					};

				const std::unordered_map<std::wstring, Value>& unorder_map = *(m_Value.pObj);

				if (isOrder)
				{
					std::map<std::wstring, Value> map;
					for (auto& value : unorder_map)
					{
						map.insert(value);
					}

					for (const auto& value : map)
					{
						fn_proce_one(value);
					}
				}
				else
				{

					for (const auto& value : unorder_map)
					{
						fn_proce_one(value);
					}
				}

			}
			count--;

			// if not null object
			if (wsText.back() == L',')
			{
				wsText.pop_back();
				if (isFormat)
				{
					wsText.append(1, L'\n');
					wsText.append(count, L'\t');
				}
			}

			wsText.append(1, L'}');
		}
		break;
		}
	}
}

namespace Rut::RxJson
{
	Parser::Parser()
	{
		m_wpJson = nullptr;
		m_nJsonCCH = 0;
		m_nReadCCH = 0;
	}

	Parser::Parser(std::wstring_view wsJson) : Parser()
	{
		this->Open(wsJson);
	}

	Parser::Parser(std::wstring_view wsJson, Value& rfJValue) : Parser(wsJson)
	{
		this->Read(rfJValue);
	}

	Parser::~Parser()
	{
		if (m_wpJson)
		{
			delete[] m_wpJson;
		}
	}


	void Parser::AddReadCCH(size_t nCount)
	{
		this->m_nReadCCH += nCount;
	}

	wchar_t Parser::GetCurChar()
	{
		return m_wpJson[m_nReadCCH];
	}

	wchar_t* Parser::GetCurPtr()
	{
		return m_wpJson + m_nReadCCH;
	}

	size_t Parser::GeReadCCH() const
	{
		return m_nReadCCH;
	}

	size_t Parser::GetJsonCCH() const
	{
		return this->m_nJsonCCH;
	}


	wchar_t Parser::SkipWhite()
	{
		while (true)
		{
			wchar_t tmp_char = this->GetCurChar();
			switch (tmp_char)
			{
			case L'\t': case L'\n':case L'\r':
			case L' ': this->AddReadCCH(); continue;
			}
			return tmp_char;
		}
	}

	wchar_t Parser::GetToken()
	{
		wchar_t tmp_char = this->SkipWhite();

		switch (tmp_char)
		{
		case L'{':case L'}':
		case L'[':case L']':
		case L'"':case L':':
		case L'n':case L't':case L'f':
		case L'0':case L'1':case L'2':
		case L'3':case L'4':case L'5':
		case L'6':case L'7':case L'8':
		case L'9':case L'-':return tmp_char;
		case L',':this->AddReadCCH(); return this->GetToken();
		default: throw std::runtime_error("Not Find Token");
		}
	}


	void Parser::ParseArray(Value& rfJValue)
	{
		assert(this->GetCurChar() == L'[');

		this->AddReadCCH();

		while (this->GetToken() != L']')
		{
			Value value;
			this->ParseValue(value);
			rfJValue.Append(std::move(value));
		}

		this->AddReadCCH();

		rfJValue.SureArray();
		return;
	}

	void Parser::ParseObject(Value& rfJValue)
	{
		assert(this->GetCurChar() == L'{');

		this->AddReadCCH();

		std::wstring_view key;
		while (true)
		{
			switch (this->GetToken())
			{
			case L'"': // parse key
			{
				this->AddReadCCH();

				wchar_t* beg = this->GetCurPtr();
				wchar_t* end = ::wcschr(beg, L'"');
				key = { beg, end };

				this->AddReadCCH((end - beg) + 1);
			}
			break;

			case L':': // check key : value
			{
				this->AddReadCCH();
				this->SkipWhite();

				Value value;
				this->ParseValue(value);
				rfJValue.AddKey(key, std::move(value));
			}
			break;

			case L'}': // end of object
			{
				this->AddReadCCH();
				rfJValue.SureObject();
				return;
			}
			break;

			default: { throw std::runtime_error("ParseObject Error!"); }
			}
		}
	}

	void Parser::ParseNumber(Value& rfJValue)
	{
		wchar_t* end = nullptr;
		wchar_t* beg = this->GetCurPtr();
		double num_org = ::wcstod(beg, &end);

		this->AddReadCCH(end - beg);

		int num_int = (int)(num_org);
		double num_loss = (double)(num_int);

		num_org == num_loss ? (rfJValue = (int)num_int) : (rfJValue = num_org);
	}

	void Parser::ParseString(Value& rfJValue)
	{
		auto fn_make_unicode = [](const wchar_t* wpStr) -> uint32_t
			{
				auto fn_char2num = [](wchar_t cChar) -> uint8_t
					{
						if ((uint16_t)cChar >= 0x41) // A-Z
						{
							return (uint8_t)(cChar - 0x37);
						}
						else // 0-9
						{
							return (uint8_t)(cChar - 0x30);
						}
					};

				uint8_t hex_0 = fn_char2num(wpStr[0]);
				uint8_t hex_1 = fn_char2num(wpStr[1]);
				uint8_t hex_2 = fn_char2num(wpStr[2]);
				uint8_t hex_3 = fn_char2num(wpStr[3]);

				return (hex_0 << 12) | (hex_1 << 8) | (hex_2 << 4) | (hex_3 << 0);
			};

		assert(this->GetCurChar() == '"');

		this->AddReadCCH();

		std::wstring text;
		while (true)
		{
			wchar_t ch = this->GetCurChar();

			if (ch == L'\\') // control characters
			{
				this->AddReadCCH();
				wchar_t ctrl_char = this->GetCurChar();

				switch (ctrl_char)
				{
				case L'\\': ctrl_char = L'\\'; break;
				case L'"': ctrl_char = L'\"'; break;
				case L'b': ctrl_char = L'\b'; break;
				case L'f': ctrl_char = L'\f'; break;
				case L'n': ctrl_char = L'\n'; break;
				case L'r': ctrl_char = L'\r'; break;
				case L't': ctrl_char = L'\t'; break;
				case L'u':
				{
					this->AddReadCCH();
					ctrl_char = (wchar_t)fn_make_unicode(this->GetCurPtr());
					this->AddReadCCH(3);
				}
				break;

				default: throw std::runtime_error("Unknow Control Characters");
				}

				ch = ctrl_char;
			}
			else if (ch == L'"') // end
			{
				this->AddReadCCH(); // skip " char
				break;
			}

			text.append(1, ch);
			this->AddReadCCH(); // netx char
		}

		Value value(std::move(text));
		rfJValue = std::move(value);
	}

	void Parser::ParseTrue(Value& rfJValue)
	{
		assert(this->GetCurChar() == L't');

		this->AddReadCCH(4);
		rfJValue = true;
	}

	void Parser::ParseFalse(Value& rfJValue)
	{
		assert(this->GetCurChar() == L'f');

		this->AddReadCCH(5);
		rfJValue = false;
	}

	void Parser::ParseNull(Value& rfJValue)
	{
		assert(this->GetCurChar() == L'n');

		this->AddReadCCH(4);
		rfJValue = Value();
	}

	void Parser::ParseValue(Value& rfJValue)
	{
		switch (this->GetCurChar())
		{
		case L'{': ParseObject(rfJValue); break; // object
		case L'[': ParseArray(rfJValue);  break; // array
		case L't': ParseTrue(rfJValue);   break; // true
		case L'f': ParseFalse(rfJValue);  break; // false
		case L'n': ParseNull(rfJValue);   break; // null
		case L'"': ParseString(rfJValue); break; // string
		case L'0':case L'1':case L'2':case L'3':
		case L'4':case L'5':case L'6':case L'7':
		case L'8':case L'9':
		case L'-': ParseNumber(rfJValue); break; // Number
		default: throw std::runtime_error("Json Parse Value Error!");
		}
	}


	void Parser::Open(std::wstring_view wsJson)
	{
		std::wstring json_text;
		RxFile::Text{ wsJson, RIO_READ, RFM_UTF8 }.ReadRawText(json_text);

		this->m_nJsonCCH = json_text.size();
		this->m_wpJson = new wchar_t[m_nJsonCCH + 1];
		wcsncpy_s(this->m_wpJson, m_nJsonCCH + 1, json_text.data(), this->m_nJsonCCH);
		this->m_wpJson[this->m_nJsonCCH] = L'\0';
	}

	bool Parser::Read(Value& rfJValue)
	{
		this->ParseValue(rfJValue);

		this->SkipWhite();
		return (this->GeReadCCH() >= this->GetJsonCCH()) ? (true) : (false);
	}

	bool Parser::Load(std::wstring_view wsJson, Value& rfJValue)
	{
		this->Open(wsJson);
		return this->Read(rfJValue);
	}

	void Parser::Save(Value& rfJVaue, std::wstring_view wsFileName, bool isFormat, bool isOrder)
	{
		std::wstring text;
		rfJVaue.Dump(text, isFormat, isOrder);
		RxFile::Text{ wsFileName ,RIO_WRITE, RFM_UTF8 }.WriteLine(text);
	}
}