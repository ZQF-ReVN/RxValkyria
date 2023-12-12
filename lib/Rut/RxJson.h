#pragma once
#include <string>
#include <vector>
#include <unordered_map>


namespace Rut::RxJson
{
	class Value;

	using JBool = bool;
	using JInt = int;
	using JDouble = double;
	using JString = std::wstring;
	using JArray = std::vector<Value>;
	using JObject = std::unordered_map<std::wstring, Value>;

	enum JValue_Type
	{
		JVALUE_NUL,
		JVALUE_BOL,
		JVALUE_INT,
		JVALUE_DBL,
		JVALUE_STR,
		JVALUE_ARY,
		JVALUE_OBJ
	};

	typedef union
	{
		JBool Bool;
		JInt Int;
		JDouble Double;
		JString* pStr;
		JArray* pAry;
		JObject* pObj;
	}JValue_Value;


	class Value
	{
	private:
		JValue_Type m_Type;
		JValue_Value m_Value;

	public:
		Value();
		Value(bool bValue);
		Value(int iValue);
		Value(double dValue);
		Value(const wchar_t* sValue);
		Value(std::wstring_view sValue);
		Value(const std::wstring& sValue);
		Value(std::wstring&& sValue);
		Value(const JArray& aValue);
		Value(JArray&& aValue);
		Value(const JObject& oValue);
		Value(JObject&& oValue);
		Value(const Value& rfJValue);
		Value(Value&& rfJValue) noexcept;
		~Value();

		Value& Copy(const Value& rfJValue);
		Value& Move(Value& rfJValue);
		Value& operator = (const Value& rfJValue);
		Value& operator = (Value&& rfJValue) noexcept;

		//Array
		void SureArray();
		void Append(const Value& rfJValue);
		void Append(Value&& rfJValue);

		// Obj
		Value& operator[](const wchar_t* wpKey);
		void SureObject();
		void AddKey(std::wstring_view wsKey);
		void AddKey(std::wstring_view wsKey, const Value& rfJValue);
		void AddKey(std::wstring_view wsKey, Value&& rfJValue);
		bool HasKey(std::wstring_view wsKey);

		JValue_Type GetType();

		operator bool();
		operator int();
		operator double();
		operator JString& ();
		operator JArray& ();
		operator JObject& ();
		operator std::wstring_view();

		JArray& ToAry();
		JObject& ToOBJ();
		int ToInt();
		bool ToBool();
		double ToDouble();
		const wchar_t* ToStringPtr();
		std::wstring_view ToStringView();

		void Dump(std::wstring& wsText, bool isFormat = true, bool isOrder = false) const;
	};

}

namespace Rut::RxJson
{
	class Parser
	{
	private:
		wchar_t* m_wpJson;
		size_t m_nJsonCCH;
		size_t m_nReadCCH;

	public:
		Parser();
		Parser(std::wstring_view wsJson);
		Parser(std::wstring_view wsJson, Value& rfJValue);
		~Parser();

	private:
		wchar_t SkipWhite();
		wchar_t GetToken();

		void AddReadCCH(size_t nCount = 1);
		wchar_t GetCurChar();
		wchar_t* GetCurPtr();
		size_t GeReadCCH() const;
		size_t GetJsonCCH() const;

		void ParseArray(Value& rfJValue);
		void ParseObject(Value& rfJValue);
		void ParseNumber(Value& rfJValue);
		void ParseString(Value& rfJValue);
		void ParseTrue(Value& rfJValue);
		void ParseFalse(Value& rfJValue);
		void ParseNull(Value& rfJValue);

		void ParseValue(Value& rfJValue);

	public:
		void Open(std::wstring_view wsJson);
		bool Read(Value& rfJValue);
		bool Load(std::wstring_view wsJson, Value& rfJValue);
		static void Save(Value& rfJVaue, std::wstring_view wsFileName, bool isFormat = false, bool isOrder = false);
	};
}