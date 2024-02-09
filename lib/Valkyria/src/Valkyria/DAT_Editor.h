#pragma once
#include <string>
#include <filesystem>


namespace Valkyria::DAT
{
	bool Unpack(const std::filesystem::path& phPack, const std::filesystem::path& phExtract);
	bool Repack(const std::filesystem::path& phFolder, const std::filesystem::path& phNewPack);
}