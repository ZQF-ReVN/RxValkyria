#include <ranges>
#include <print>
#include <iostream>
#include <string_view>
#include <ZxFS/Core.h>
#include <ZxFS/Walker.h>
#include <ZxJson/JIO.h>
#include <ZxJson/JValue.h>
#include <RxValkyria/Core/SDT_HDR.h>
#include <RxValkyria/Core/SDT_TextParser.h>


static auto Export(const std::string_view msSdtPath, const std::string_view msSavePath, const std::size_t nCodePage) -> bool
{
	ZQF::RxValkyria::SDT::TextParserJsonManager text_parser{ msSdtPath };
	if (text_parser.Parse())
	{
		text_parser.Export(msSavePath,nCodePage);
		return true;
	}
	return false;
}

static auto Import(const std::string_view msSdtPath, const std::string_view msJsonPath, const std::string_view msNewSdtPath, const std::size_t nCodePage) -> bool
{
	ZQF::RxValkyria::SDT::TextParserJsonManager text_parser{ msSdtPath };
	if (text_parser.Parse())
	{
		text_parser.Import(msNewSdtPath, msJsonPath, nCodePage);
		return true;
	}

	return false;
}

static auto TestHDRParser() -> void
{
	std::string_view sdt_files_org_dir{ "sdt_org/" };
	ZQF::ZxFS::DirMake(sdt_files_org_dir, false);
	for (ZQF::ZxFS::Walker walk{ sdt_files_org_dir }; walk.NextFile(); )
	{
		ZQF::ZxMem sdt_mem{ walk.GetPath() };

		ZQF::RxValkyria::SDT::HDR hdr;
		hdr.Load(sdt_mem.Ptr());

		const auto hdr_size_bytes = hdr.SizeBytes();

		ZQF::ZxMem hdr_mem{ hdr_size_bytes };
		hdr.Make(hdr_mem);

		if (std::memcmp(hdr_mem.Ptr(), sdt_mem.PtrCur(), hdr_size_bytes != 0))
		{
			std::println("FAILED: {}", walk.GetPath());
		}
	}

	std::println("Passed -> HDR Parser");
}

static auto TestSdtTextParser() -> void
{
	std::size_t export_code_page{ 932 };
	std::string_view sdt_files_org_dir{ "sdt_org/" };
	std::string_view sdt_files_export_dir{ "sdt_textobj/" };

	ZQF::ZxFS::DirMake(sdt_files_org_dir, false);
	ZQF::ZxFS::DirMake(sdt_files_export_dir, false);

	for (ZQF::ZxFS::Walker walk{ sdt_files_org_dir }; walk.NextFile(); )
	{
		const auto sdt_org_path = walk.GetPath();

		std::string sdt_export_path{ sdt_files_export_dir };
		sdt_export_path.append(ZQF::ZxFS::FileSuffixDel(walk.GetName())).append(".json");

		ZQF::RxValkyria::SDT::TextParser parser{ sdt_org_path };
		ZQF::ZxJson::JValue obj_json = parser.MakeJson(export_code_page);
		ZQF::ZxJson::StoreViaFile(sdt_export_path, obj_json, true, true);
	}

	std::println("Passed -> TextParser");
}

// tested: 2024.8.27 : VAL-0065	プレイ！プレイ！プレイ！アンロック！
// tested: 2024.8.27 : VAL-0064
static auto TestTextEditor() -> void
{
	std::size_t export_code_page{ 932 };
	std::size_t import_code_page{ 936 };
	std::string_view sdt_files_org_dir{ "sdt_org/" };
	std::string_view sdt_files_export_dir{ "sdt_export/" };
	std::string_view sdt_files_import_dir{ "sdt_import/" };

	ZQF::ZxFS::DirMake(sdt_files_org_dir, false);
	ZQF::ZxFS::DirMake(sdt_files_export_dir, false);
	ZQF::ZxFS::DirMake(sdt_files_import_dir, false);

	for (ZQF::ZxFS::Walker walk{ sdt_files_org_dir }; walk.NextFile(); )
	{
		const auto sdt_org_path = walk.GetPath();

		std::string sdt_export_path{ sdt_files_export_dir };
		sdt_export_path.append(ZQF::ZxFS::FileSuffixDel(walk.GetName())).append(".json");

		std::string sdt_import_path{ sdt_files_import_dir };
		sdt_import_path.append(walk.GetName());

		if (::Export(sdt_org_path, sdt_export_path, export_code_page))
		{
			::Import(sdt_org_path, sdt_export_path, sdt_import_path, import_code_page);
		}
		else
		{
			std::println("Not Find Text: {}", sdt_org_path);
		}
	}

	std::println("Passed -> TextEditor");
}


auto main(void) -> int
{
	try
	{
		// TestHDRParser();
		TestSdtTextParser();
		// TestTextEditor();
	}
	catch (const std::exception& err)
	{
		std::println(std::cerr, "std::exception: {}", err.what());
	}
}
