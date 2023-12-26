#include <iostream>
#include <filesystem>

#include "../../lib/Valkyria/SDT.h"


static bool ParseSdtText(Valkyria::SDT::Text_Parser& text_parser, std::wstring_view wsSdtPath)
{
	text_parser.Read(wsSdtPath);
	if (text_parser.GetSdtFile().GetMsgCount() == 0) { return false; }
	text_parser.Scan();
	if (text_parser.GetMsgCount() == 0) { return false; }
	return true;
}

static void FilterText(Rut::RxJson::JArray& rfObjectJson, Rut::RxJson::JArray& rfTextJson)
{
	Rut::RxJson::JArray text_list;
	Rut::RxJson::JArray index_list;

	size_t obj_count = rfObjectJson.size();
	for (size_t ite = 0; ite < obj_count; ite++)
	{
		Rut::RxJson::JValue& obj = rfObjectJson[ite];
		std::wstring_view obj_name = obj[L"Name"];

		if (obj_name == L"MsgText" ||
			obj_name == L"MsgName" ||
			obj_name == L"SelectText" ||
			obj_name == L"StrSet")
		{
			Rut::RxJson::JValue text_entry;
			text_entry[L"org"] = obj[L"Text"];
			text_entry[L"tra"] = obj[L"Text"];
			text_list.push_back(std::move(text_entry));

			index_list.push_back((int)ite);
		}
	}

	rfTextJson.push_back(std::move(text_list));
	rfTextJson.push_back(std::move(index_list));
}

static void MergeText(Rut::RxJson::JArray& rfObjectJson, Rut::RxJson::JArray& rfTextJson)
{
	Rut::RxJson::JArray& obj_list = rfObjectJson;
	Rut::RxJson::JArray& text_list = rfTextJson[0];
	Rut::RxJson::JArray& index_list = rfTextJson[1];

	if (text_list.size() != index_list.size())
	{
		throw std::runtime_error("Merge Text Json Mismatching!");
	}

	size_t text_count = text_list.size();
	for (size_t ite = 0; ite < text_count; ite++)
	{
		size_t index = (size_t)index_list[ite].ToInt();
		auto& text_entry = text_list[ite];
		obj_list[index][L"Text"] = text_entry[L"tra"];
	}
}

static void Export(std::wstring_view wsSdtPath, std::wstring_view wsObjectJson, std::wstring_view wsTextJson)
{
	Valkyria::SDT::Text_Parser text_parser;
	if (ParseSdtText(text_parser, wsSdtPath) == false) { return; }

	Rut::RxJson::JValue obj_json = text_parser.Make(932);
	Rut::RxJson::Parser::Save(obj_json, wsObjectJson, true);

	Rut::RxJson::JValue txt_json;
	FilterText(obj_json, txt_json);
	Rut::RxJson::Parser::Save(txt_json, wsTextJson, true);
}

static void Import(std::wstring_view wsSdtPath, std::wstring_view wsObjectJson, std::wstring_view wsTextJson, std::wstring_view wsNewSDT)
{
	Valkyria::SDT::Text_Parser text_parser;
	if (ParseSdtText(text_parser, wsSdtPath) == false) { return; }

	Rut::RxJson::JValue obj_json = Rut::RxJson::Parser{}.Load(wsObjectJson);
	Rut::RxJson::JValue txt_json = Rut::RxJson::Parser{}.Load(wsTextJson);

	MergeText(obj_json, txt_json);
	text_parser.Load(obj_json, 936);
	text_parser.Make().SaveData(wsNewSDT);
}


static void UserMain()
{

}

static void DebugMain()
{
	std::filesystem::create_directory(L"txt/");
	std::filesystem::create_directory(L"obj/");
	std::filesystem::create_directory(L"out/");

	if (true)
	{
		for (auto& path_entry : std::filesystem::directory_iterator(L"sdt/"))
		{
			if (path_entry.is_regular_file() == false) { continue; }
			const std::filesystem::path& sdt_path = path_entry.path();
			const std::filesystem::path& obj_json_path = L"obj" / sdt_path.filename().replace_extension(L".obj.json");
			const std::filesystem::path& txt_json_path = L"txt" / sdt_path.filename().replace_extension(L".txt.json");
			Export(sdt_path.wstring(), obj_json_path.wstring(), txt_json_path.wstring());
		}
	}
	else
	{
		for (auto& path_entry : std::filesystem::directory_iterator(L"sdt/"))
		{
			if (path_entry.is_regular_file() == false) { continue; }
			const std::filesystem::path& sdt_path = path_entry.path();
			const std::filesystem::path& obj_json_path = L"obj" / sdt_path.filename().replace_extension(L".obj.json");
			const std::filesystem::path& txt_json_path = L"txt" / sdt_path.filename().replace_extension(L".txt.json");
			if (std::filesystem::exists(obj_json_path) == false) { continue; }
			const std::filesystem::path& new_std_path = L"make" / sdt_path.filename();
			Import(sdt_path.wstring(), obj_json_path.wstring(), txt_json_path.wstring(), new_std_path.wstring());
		}
	}
}

int main()
{
	DebugMain();
	try
	{
		
	}
	catch (const std::runtime_error& err)
	{
		std::cerr << err.what() << std::endl;
	}
}
