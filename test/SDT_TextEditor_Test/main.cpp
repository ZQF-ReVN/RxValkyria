#include <ranges>
#include <iostream>
#include <filesystem>

#include <Valkyria/SDT_TextParser.h>


static bool Export(const std::filesystem::path& phSdt, const std::filesystem::path& phTextJson, size_t nCodePage)
{
	Valkyria::SDT::TextParser text_parser(phSdt);
	if (text_parser.ParseText())
	{
		Rut::RxJson::JValue txt_json = text_parser.ReadText(nCodePage);
		Rut::RxJson::Parser::Save(txt_json, phTextJson, true);
		return true;
	}
	return false;
}

static bool Import(const std::filesystem::path& phSdt, const std::filesystem::path& phTextJson, const std::filesystem::path& phNewSdt, size_t nCodePage)
{
	Valkyria::SDT::TextParser text_parser(phSdt);
	if (text_parser.ParseText())
	{
		text_parser.LoadText(phTextJson, nCodePage);
		text_parser.Make().SaveData(phNewSdt);
		return true;
	}
	return false;
}


// tested: VAL-0065	[]プレイ！プレイ！プレイ！アンロック！
static void DebugMain()
{
	size_t export_code_page = 932;
	size_t import_code_page = 936;
	std::filesystem::path sdt_files_org_dir = L"sdt_org/";
	std::filesystem::path sdt_files_export_dir = L"sdt_export/";
	std::filesystem::path sdt_files_import_dir = L"sdt_import/";

	std::filesystem::create_directories(sdt_files_org_dir);
	std::filesystem::create_directories(sdt_files_export_dir);
	std::filesystem::create_directories(sdt_files_import_dir);

	for (const auto& path_entry : std::filesystem::directory_iterator(sdt_files_org_dir))
	{
		if (path_entry.is_regular_file() == false) { continue; }

		const std::filesystem::path& sdt_org_path = path_entry.path();
		std::filesystem::path sdt_export_path = sdt_files_export_dir / sdt_org_path.filename().replace_extension(L".json");
		std::filesystem::path sdt_import_path = sdt_files_import_dir / sdt_org_path.filename();

		if (::Export(sdt_org_path, sdt_export_path, export_code_page))
		{
			// ::Import(sdt_org_path, sdt_export_path, sdt_import_path, import_code_page);
		}
		else
		{
			std::cout << "Not Find Text: " << sdt_org_path << "\n";
		}
	}

	std::cout << "Passed!\n";

}

int main()
{
	DebugMain();
	try
	{
		
	}
	catch (const std::exception& err)
	{
		std::cerr << err.what() << std::endl;
	}
}
