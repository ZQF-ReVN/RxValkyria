#include <print>
#include <iostream>
#include <string_view>
#include <ZxFS/Core.h>
#include <ZxFS/Walker.h>
#include <RxValkyria/Core/Pack.h>

// the oldest tested: VAL-045	[2018-05-25] プレイ！プレイ！プレイ！GO！ DL
// the latest tested: VAL-0066	[2023-11-24] 三美姫の牝豚 ～姦獄の城～ DL

auto main(void) -> int
{
	try
	{
		std::string_view dat_files_org_dir{ "dat_org/" };
		std::string_view dat_files_import_dir{ "dat_import/" };
		std::string_view dat_files_export_dir{ "dat_export/" };

		ZQF::ZxFS::DirMake(dat_files_org_dir, false);
		ZQF::ZxFS::DirMake(dat_files_import_dir, false);
		ZQF::ZxFS::DirMake(dat_files_export_dir, false);

		for (ZQF::ZxFS::Walker walk{ dat_files_org_dir }; walk.NextFile();)
		{
			const auto pack_path = walk.GetPath();

			std::string export_dir{ dat_files_export_dir };
			export_dir.append(ZQF::ZxFS::FileNameStem(walk.GetName()));
			export_dir.append(1, '/');

			std::string new_pack_path{ dat_files_import_dir };
			new_pack_path.append(walk.GetName());

			ZQF::RxValkyria::Pack::Export(pack_path, export_dir);
			ZQF::RxValkyria::Pack::Import(export_dir, new_pack_path);

			std::println("Success: {}", pack_path);
		}

		std::println("Passed!");
	}
	catch (const std::exception& err)
	{
		std::println(std::cerr, "std::exception: {}", err.what());
	}
}