#include "Pack.h"
#include "Valkyria_Struct.h"
#include <Zut/ZxMem.h>
#include <Zut/ZxFile.h>
#include <Zut/ZxCvt.h>
#include <Zut/ZxFS.h>
#include <cstdint>
#include <vector>
#include <ranges>


namespace ZQF::ReVN::RxValkyria
{
	auto Pack::Export(const std::string_view msPackPath, const std::string_view msExportDir) -> bool
	{
		ZxFile pack_ifs{ msPackPath, ZxFile::OpenMod::ReadSafe };

		const auto index_size{ pack_ifs.Get<std::uint32_t>() };

		ZxMem index_table_mem(index_size);
		pack_ifs.Read(index_table_mem.Span());

		ZxCvt cvt;
		ZxMem buffer;
		std::string path_tmp;
		for (auto& entry : index_table_mem.Span<Struct::Pack_Entry>())
		{
			const auto file_foa = static_cast<std::size_t>(entry.nOffset) + index_size + sizeof(index_size);
			const auto file_size = static_cast<std::size_t>(entry.nSize);
			pack_ifs.Seek(file_foa, ZxFile::MoveWay::Set);
			pack_ifs.Read(buffer.Resize(file_size).Span());

			const auto& save_path = path_tmp.append(msExportDir).append(cvt.MBCSToUTF8(entry.aFileName, 932));
			buffer.Save(save_path);
			path_tmp.clear();
		}

		return true;
	}


	auto Pack::Import(const std::string_view msImportDir, const std::string_view msNewPackPath) -> bool
	{
		std::vector<std::pair<std::string, std::string>> file_info_list;
		for (ZxFS::Walker walk{ msImportDir }; walk.NextFile(); )
		{
			file_info_list.emplace_back(std::pair{ std::string{ walk.GetName() }, walk.GetPath() });
		}

		if (file_info_list.empty()) { throw std::runtime_error(std::string{ "Valkyria::DAT::Pack: not find file! -> msImportDir: " }.append(msImportDir)); }

		const auto index_table_bytes = file_info_list.size() * Struct::Pack_Entry::SizeBytes;

		ZxFile pack_ofs { msNewPackPath, ZxFile::OpenMod::WriteForce };

		pack_ofs << static_cast<std::uint32_t>(index_table_bytes);

		ZxMem cache_mem;

		auto& index_table_mem = cache_mem.Resize(index_table_bytes);
		{
			ZxCvt cvt;
			std::size_t cur_pos{};
			for (auto& file_info : file_info_list)
			{
				const auto file_size_opt = ZxFS::FileSize(file_info.second);
				if (!file_size_opt.has_value()) { throw std::runtime_error("get file size error. -> "); }

				const auto file_name_sjis = cvt.UTF8ToMBCS(file_info.first, 932);

				auto entry_ptr = index_table_mem.PtrCur<Struct::Pack_Entry*>();
				index_table_mem.PosInc(Struct::Pack_Entry::SizeBytes);
				
				memcpy(entry_ptr->aFileName, file_name_sjis.data(), file_name_sjis.size());
				entry_ptr->aFileName[file_name_sjis.size()] = {};
				entry_ptr->nOffset = static_cast<std::uint32_t>(cur_pos);
				entry_ptr->nSize = static_cast<std::uint32_t>(*file_size_opt);

				cur_pos += static_cast<std::size_t>(*file_size_opt);
			}

			pack_ofs.Write(index_table_mem.Span());
		}

		auto& file_buffer = cache_mem;
		for (const auto& file_info : file_info_list)
		{
			file_buffer.Load(file_info.second);
			pack_ofs.Write(file_buffer.Span());
		}

		return true;
	}
}