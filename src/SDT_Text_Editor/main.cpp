#include <iostream>
#include <filesystem>


#include "../../lib/Rut/RxStr.h"
#include "../../lib/Rut/RxConsole.h"
#include "../../lib/Valkyria/SDT.h"


static void Extrct(std::wstring_view wsSDT, std::wstring_view wsJson)
{
	Valkyria::SDT::SDT_Parser parse(wsSDT);
	Valkyria::SDT::STD_Text_Test text_t;
	text_t.Parse(parse.GetCodeMem());

	Valkyria::SDT::STD_Text text(wsSDT);
	if (text.GetMsgCount())
	{
		Rut::RxJson::Value json_text = text.ToJson();
		Rut::RxJson::Parser::Save(json_text, wsJson, true);
	}
}

static void Import(std::wstring_view wsSDT, std::wstring_view wsJson)
{
	Valkyria::SDT::STD_Text text(wsSDT);
	text.LoadViaJson(wsJson, 932);
}

int main()
{
	try
	{
		std::filesystem::create_directory(L"json/");

		for (auto& path_entry : std::filesystem::directory_iterator(L"sdt/"))
		{
			if (path_entry.is_regular_file() == false) { continue; }
			const std::filesystem::path& sdt_path = path_entry.path();
			const std::filesystem::path& json_path = L"json" / sdt_path.filename().replace_extension(L".json");
			if (std::filesystem::exists(json_path))
			{
				Extrct(sdt_path.wstring(), json_path.wstring());
			}

		}
	}
	catch (const std::runtime_error& err)
	{
		std::cerr << err.what() << std::endl;
	}
}
