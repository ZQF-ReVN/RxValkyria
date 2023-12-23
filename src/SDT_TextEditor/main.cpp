#include <iostream>
#include <filesystem>

#include "../../lib/Valkyria/SDT.h"


static void Export(std::wstring_view wsSDT, std::wstring_view wsJson)
{
	Valkyria::SDT::Text_Parser text_parser;
	text_parser.Init(wsSDT);
	if (text_parser.GetMsgCountViaInfo() == 0) { return; }
	text_parser.Parse();
	if (text_parser.GetMsgCountViaScan() == 0) { return; }
	Rut::RxJson::JValue json_text = text_parser.ToJson(932, true);
	Rut::RxJson::Parser::Save(json_text, wsJson, true);
}

static void Import(std::wstring_view wsSDT, std::wstring_view wsJson, std::wstring_view wsNewSDT)
{
	Rut::RxJson::JValue json;
	Rut::RxJson::Parser parser(wsJson, json);

	Valkyria::SDT::Text_Parser text_parser;
	text_parser.Init(wsSDT);
	text_parser.Parse();
	text_parser.Load(json.ToAry(), 936);
	text_parser.Make().SaveData(wsNewSDT);
}


static void UserMain()
{

}

static void DebugMain()
{
	std::filesystem::create_directory(L"json/");
	std::filesystem::create_directory(L"make/");

	//for (auto& path_entry : std::filesystem::directory_iterator(L"sdt/"))
	//{
	//	if (path_entry.is_regular_file() == false) { continue; }
	//	const std::filesystem::path& sdt_path = path_entry.path();
	//	const std::filesystem::path& json_path = L"json" / sdt_path.filename().replace_extension(L".json");
	//	Export(sdt_path.wstring(), json_path.wstring());
	//}

	for (auto& path_entry : std::filesystem::directory_iterator(L"sdt/"))
	{
		if (path_entry.is_regular_file() == false) { continue; }
		const std::filesystem::path& sdt_path = path_entry.path();
		const std::filesystem::path& json_path = L"json" / sdt_path.filename().replace_extension(L".json");
		const std::filesystem::path& new_std_path = L"make" / sdt_path.filename();
		if (std::filesystem::exists(json_path))
		{
			Import(sdt_path.wstring(), json_path.wstring(), new_std_path.wstring());
		}
	}
}

int main()
{
	try
	{
		DebugMain();
	}
	catch (const std::runtime_error& err)
	{
		std::cerr << err.what() << std::endl;
	}
}
