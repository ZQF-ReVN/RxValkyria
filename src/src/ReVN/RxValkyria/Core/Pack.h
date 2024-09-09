#pragma once
#include <string_view>


namespace ZQF::ReVN::RxValkyria
{
	class Pack
	{
	public:
		static auto Export(const std::string_view msPackPath, const std::string_view msExportDir) -> bool;
		static auto Import(const std::string_view msImportDir, const std::string_view msNewPackPath) -> bool;
	};

}