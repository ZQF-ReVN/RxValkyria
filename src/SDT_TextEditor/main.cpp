#include <ranges>
#include <iostream>
#include <filesystem>

#include <Rut/RxCmd.h>
#include <Valkyria/SDT_TextParser.h>


static void Export(const std::filesystem::path& phSdt, const std::filesystem::path& phTextJson, size_t nCodePage)
{
	Valkyria::SDT::TextParser text_parser(phSdt);
	if (text_parser.ParseText())
	{
		Rut::RxJson::JValue txt_json = text_parser.ReadText(nCodePage);
		Rut::RxJson::Parser::Save(txt_json, phTextJson, true);
	}
	else
	{
		std::cout << "Not Find Text: " << phSdt << "\n";
	}
}

static void Import(const std::filesystem::path& phSdt, const std::filesystem::path& phTextJson, const std::filesystem::path& phNewSdt, size_t nCodePage)
{
	Valkyria::SDT::TextParser text_parser(phSdt);
	if (text_parser.ParseText())
	{
		text_parser.LoadText(phTextJson, nCodePage);
		text_parser.Make().SaveData(phNewSdt);
	}
	else
	{
		std::cout << "Not Find Text: " << phSdt << "\n";
	}
}

static void UserMain(int argc, wchar_t* argv[])
{
	try
	{
		Rut::RxCmd::Arg arg;
		arg.AddCmd(L"-sdt", L"sdt org path");
		arg.AddCmd(L"-json", L"json path");
		arg.AddCmd(L"-new", L"sdt new path");
		arg.AddCmd(L"-mode", L"mode: [single] [batch]");
		arg.AddCmd(L"-way", L"way: [export] [import]");
		arg.AddCmd(L"-code", L"code page");
		arg.AddExample(L"-mode batch -way export -sdt sdt/ -json sdt_json/ -code 932");
		arg.AddExample(L"-mode batch -way import -sdt sdt/ -json sdt_json/ -new sdt_new/ -code 936");
		arg.AddExample(L"-mode single -way export -sdt seen1100.sdt -json seen1100.json -code 932");
		arg.AddExample(L"-mode single -way import -sdt seen1100.sdt -json seen1100.json -new seen1100.sdt.new -code 936");
		if (arg.Load(argc, argv) == false) { return; }

		if (arg[L"-mode"] == L"batch")
		{
			size_t code_page = arg[L"-code"];
			std::filesystem::path sdt_files_dir = arg[L"-sdt"];
			std::filesystem::path json_files_dir = arg[L"-json"];
			if (arg[L"-way"] == L"export")
			{
				std::filesystem::create_directories(json_files_dir);
				for (auto& path_entry : std::filesystem::directory_iterator(sdt_files_dir))
				{
					if (path_entry.is_regular_file() == false) { continue; }
					const std::filesystem::path& sdt_path = path_entry.path();
					std::filesystem::path json_path = json_files_dir / sdt_path.filename().replace_extension(L".json");
					::Export(sdt_path, json_path, code_page);
				}
			}
			else if (arg[L"-way"] == L"import")
			{
				std::filesystem::path sdt_files_new_dir = arg[L"-new"];
				std::filesystem::create_directories(sdt_files_new_dir);
				for (auto& path_entry : std::filesystem::directory_iterator(json_files_dir))
				{
					if (path_entry.is_regular_file() == false) { continue; }
					const std::filesystem::path& json_path = path_entry.path();
					std::filesystem::path sdt_path = sdt_files_dir / json_path.filename().replace_extension(L".sdt");
					std::filesystem::path sdt_new_path = sdt_files_new_dir / json_path.filename().replace_extension(L".sdt");
					::Import(sdt_path, json_path, sdt_new_path, code_page);
				}
			}
		}
		else if (arg[L"-mode"] == L"single")
		{
			if (arg[L"-way"] == L"export")
			{
				::Export(arg[L"-sdt"], arg[L"-json"], arg[L"-code"]);
			}
			else if (arg[L"-way"] == L"import")
			{
				::Import(arg[L"-sdt"], arg[L"-json"], arg[L"-new"], arg[L"-code"]);
			}
		}

		std::cout << "\n\tAll Done!\n";
	}
	catch (const std::exception& err)
	{
		std::cerr << err.what() << std::endl;
	}
}

int wmain(int argc, wchar_t* argv[])
{
	::UserMain(argc, argv);
}