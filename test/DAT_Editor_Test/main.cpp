#include <iostream>

#include <Rut/RxCmd.h>
#include <Valkyria/DAT_Editor.h>


// the oldest tested: VAL-045	[2018-05-25] プレイ！プレイ！プレイ！GO！ DL
// the latest tested: VAL-0066	[2023-11-24] 三美姫の牝豚 ～姦獄の城～ DL
static void DebugMain()
{
	try
	{
		std::filesystem::path dat_files_org_dir = L"dat_org/";
		std::filesystem::path dat_files_repack_dir = L"dat_repack/";
		std::filesystem::path dat_files_extract_dir = L"dat_extract/";

		std::filesystem::create_directories(dat_files_org_dir);
		std::filesystem::create_directories(dat_files_repack_dir);
		std::filesystem::create_directories(dat_files_extract_dir);

		for (auto& path_entry : std::filesystem::directory_iterator(dat_files_org_dir))
		{
			if (path_entry.is_regular_file() == false) { continue; }

			const std::filesystem::path& dat_org_path = path_entry.path();
			std::filesystem::path dat_extract_dir = dat_files_extract_dir / dat_org_path.stem();
			std::filesystem::path dat_repack_path = dat_files_repack_dir / dat_org_path.filename();

			Valkyria::DAT::Unpack(dat_org_path, dat_extract_dir);
			Valkyria::DAT::Repack(dat_extract_dir, dat_repack_path);

			std::cout << "Success:" << dat_org_path << "\n";
		}

		std::cout << "Passed!" << std::endl;
	}
	catch (const std::exception& err)
	{
		std::cerr << err.what() << std::endl;
	}
}

int wmain(int argc, wchar_t* argv[])
{
	::DebugMain();
}