#include <iostream>

#include "../../lib/Rut/RxMem.h"
#include "../../lib/Rut/RxStr.h"
#include "../../lib/Rut/RxPath.h"
#include "../../lib/Rut/RxConsole.h"


static std::string ValkyrisReadString(uint8_t* pStr)
{
	std::string str;
	while (true)
	{
		uint8_t tmp_char = pStr++[0] ^ 0xFF;
		if (tmp_char == '\0') { break; }
		str.append(1, (char)tmp_char);
	}
	return str;
}


class SDT_Label
{
private:
	uint8_t* m_pLablePtr;
	std::wstring m_wsLabel;

public:
	SDT_Label() : m_pLablePtr(nullptr)
	{

	}

	SDT_Label(uint8_t* pLablePtr, std::wstring_view wsLabel)
	{
		m_pLablePtr = pLablePtr;
		m_wsLabel = wsLabel;
	}

	size_t Init(uint8_t* pEntry, uint8_t* pSDT, uint32_t uiHdrSize)
	{
		std::string msLabel = ValkyrisReadString(pEntry);
		m_wsLabel = Rut::RxStr::ToWCS(msLabel, 932);
		uint32_t label_rva = *(uint32_t*)(pEntry + msLabel.size() + 1);
		m_pLablePtr = pSDT + uiHdrSize + label_rva;
		return msLabel.size() + 1 + 4;
	}

	uint8_t* GetLabelPtr()
	{
		return m_pLablePtr;
	}

	std::wstring GetName()
	{
		return m_wsLabel;
	}

};


class SDT
{
private:


	Rut::RxMem::Auto m_Script;

	uint32_t m_uiHeaderSize;
	uint32_t m_uiUn0;
	uint32_t m_uiUn1;
	uint32_t m_uiLabelCount;
	uint32_t m_uiCheckDataFOA;
	std::vector<SDT_Label> m_vcLabels;
	uint8_t m_aCheckData[9];

public:
	SDT() : m_aCheckData{ 0 }
	{
		m_uiHeaderSize = 0;
		m_uiUn0 = 0;
		m_uiUn1 = 0;
		m_uiLabelCount = 0;
		m_uiCheckDataFOA = 0;
	}

	void Init(std::string_view msScript)
	{
		m_Script.LoadFile(msScript);

		uint32_t* sdt_ptr = (uint32_t*)m_Script.GetPtr();
		m_uiHeaderSize = sdt_ptr++[0];
		m_uiUn0 = sdt_ptr++[0];
		m_uiUn1 = sdt_ptr++[0];
		m_uiLabelCount = sdt_ptr++[0];
		m_uiCheckDataFOA = sdt_ptr++[0];

		if (m_uiLabelCount == 0)
		{
			m_vcLabels.emplace_back(m_Script.GetPtr() + 0x1D, L"Start");
		}
		else
		{
			uint8_t* label_entry_ptr = (uint8_t*)sdt_ptr;
			for (size_t ite_label = 0; ite_label < m_uiLabelCount; ite_label++)
			{
				SDT_Label label;
				label_entry_ptr += label.Init(label_entry_ptr, m_Script.GetPtr(), m_uiHeaderSize);
				m_vcLabels.emplace_back(std::move(label));
			}

		}

		memcpy(m_aCheckData, m_Script.GetPtr() + m_uiCheckDataFOA, sizeof(m_aCheckData));
	}

	std::vector<SDT_Label>& GetLabels()
	{
		return m_vcLabels;
	}

	uint8_t* EndPtr()
	{
		return m_Script.GetPtr() + m_Script.GetSize();
	}
};


class STD_Parser
{
private:
	uint8_t* m_pCode;
	uint8_t* m_pEnd;
	uint32_t m_uiEP;

public:
	STD_Parser(uint8_t* pCode, uint8_t* pEnd)
	{
		m_pCode = pCode;
		m_pEnd = pEnd;
		m_uiEP = 0;
	}

private:
	void AddEP(size_t nAdd)
	{
		m_uiEP += (uint32_t)nAdd;
	}

	uint16_t GetOpen()
	{
		uint16_t opcode = *(uint16_t*)(m_pCode + m_uiEP);
		AddEP(2);
		return opcode;
	}

	uint8_t* GetEP()
	{
		return (m_pCode + m_uiEP);
	}

	bool IsEnd()
	{
		if (GetEP() >= m_pEnd)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	uint8_t GetType()
	{
		uint8_t type = *GetEP();
		AddEP(1);
		return type;
	}

	uint32_t GetInt()
	{
		uint8_t type = GetType();
		switch (type)
		{
		case 0:
		{
			uint32_t value = *(uint32_t*)(this->GetEP());
			this->AddEP(4);
			return value;

		}
		break;
		}

		throw std::runtime_error("Error Type!");
	}

	std::string GetStr()
	{
		uint8_t type = GetType();
		switch (type)
		{
		case 8:
		{
			std::string label_name = ValkyrisReadString(GetEP());
			this->AddEP(label_name.size() + 1);
			return label_name;
		}
		break;
		}
		throw std::runtime_error("Error Type!");
	}

	// Call Label In All Script
	void OP_FarCallLabel()
	{
		uint32_t script_id = GetInt();
		std::string label_name = GetStr();
		std::wstring w_label_name = Rut::RxStr::ToWCS(label_name, 932);
		FarCallLabel(script_id, w_label_name);
	}

	void FarCallLabel(uint32_t uiScriptID, std::wstring wsLabel)
	{
		Rut::RxConsole::PutFormat(L"FarCallLabel(%04d,%s)\n", uiScriptID, wsLabel.c_str());
	}

	// Jmp To Script
	void OP_Jump()
	{
		uint32_t script_id = GetInt();
		JumpScript(script_id);
	}

	void JumpScript(uint32_t uiScriptID)
	{
		Rut::RxConsole::PutFormat(L"JumpScript(%04d)\n", uiScriptID);
	}

	void OP_GetSysModeNo()
	{
		uint8_t type = GetType();
		switch (type)
		{
		case 0x3:
			AddEP(4);
			break;
		}
		throw std::runtime_error("Error Type!");
	}

public:
	void Parser()
	{
		while (true)
		{
			uint16_t op = GetOpen();
			switch (op)
			{
				// Call
			case 0x0A43: break;
				// 004674E9:"[] = Num"
			case 0x0B07: break;
				// 0045C1CC: "GetSysModeNo"
			case 0x0004:OP_GetSysModeNo(); break;
				// 00468AE1: "FarCallLabel"
			case 0x0A44: OP_FarCallLabel(); break;
				// 00469432: "Jump"
			case 0x0A48: OP_Jump(); break;
			default: throw std::runtime_error("Error Opcode!");
			}

			if (IsEnd() == true)
			{
				break;
			}
		}
	}
};


class STD_Loader
{
private:
	SDT m_SDT;
	uint8_t* pEnd;

public:
	STD_Loader(std::string_view msScript)
	{
		m_SDT.Init(msScript);
		pEnd = m_SDT.EndPtr();
	}

	void ParserSDT()
	{
		auto& labels = m_SDT.GetLabels();
		for (auto& label : labels)
		{
			if (label.GetName() == L"Game初期化")
			{
				STD_Parser parser(label.GetLabelPtr(), pEnd);
				parser.Parser();
			}
		}
	}
};


int main()
{

}


