#include <iostream>
#include "../../lib/Valkyria/SDT.h"

#include "../../lib/Rut/RxStr.h"
#include "../../lib/Rut/RxPath.h"
#include "../../lib/Rut/RxConsole.h"


static void Extrct(std::wstring_view wsSDTPath, std::wstring_view wsJsonPath)
{
	Valkyria::SDT::SDT_Parser parse(wsSDTPath);
	Valkyria::SDT::STD_Text_Test text_t;
	text_t.Parse(parse.GetCodeMem());

	Valkyria::SDT::STD_Text text;
	text.Parse(parse.GetCodeMem());
	if (text.GetMsgCount())
	{
		Rut::RxJson::Value json_text = text.ToJson();
		Rut::RxJson::Parser::Save(json_text, wsJsonPath, true);
	}
}

int main()
{
	std::vector<std::wstring> file_list;
	Rut::RxPath::CurFileNames(L"sdt/",file_list);
	for (auto& file_name : file_list)
	{
		Extrct(L"sdt/" + file_name, L"json/" + file_name + L".json");
	}
}
